#! /bin/bash

# Function to display usage.
show_usage() {
    echo "Usage: $0 <wlroots-git-repository-source-tree> <wlroots-builddir>"
    echo "Pass the full path to a clean wlroots source tree, cloned from git as first argument and the build directory as second."
    exit 1
}

# Check if exactly one argument is provided.
if [ $# -ne 2 ]; then
    show_usage
fi

# Check if the argument is a directory.
if [ ! -d "$1" ]; then
    echo "Error: '$1' is not a directory."
    show_usage
fi

# Check if the directory contains required subdirectories.
if [ ! -d "$1/backend" ] || [ ! -d "$1/protocol" ] || [ ! -d "$1/render" ]; then
    show_usage
fi

# Check if the directory is a git repository.
if [ ! -d "$1/.git" ]; then
    echo "Error: '$1' is not a git repository."
    show_usage
fi

# Check if the repository is clean (no uncommitted changes).
if [ -n "$(cd "$1" && git status --porcelain --untracked-files=no)" ]; then
    echo "Error: '$1' has uncommitted changes."
    show_usage
fi

WLROOTS_DIR="$1"

# Check if $2 is a directory.
if [ ! -d "$2" ]; then
    echo "Error: '$2' is not a directory."
    show_usage
fi

# Check if $2 contains build.ninja.
if [ ! -f "$2/build.ninja" ]; then
    echo "Error: '$2' does not contain build.ninja. Please make sure to configure (and compile?) $WLROOTS_DIR first."
    show_usage
fi

# Check if $2 contains required subdirectories.
if [ ! -d "$2/backend" ] || [ ! -d "$2/protocol" ] || [ ! -d "$2/render" ]; then
    echo "'$2' doesn't look like a build directory of wlroots."
    show_usage
fi

WLROOTS_BUILDDIR=$(realpath "$2")
CPUS=$(nproc)
OUTPUT_FILE="$PWD/renamer.out"
rm -f "$OUTPUT_FILE"

echo "WLROOTS_BUILDDIR = \"${WLROOTS_BUILDDIR}\""

if ! pushd "$WLROOTS_DIR"; then
    echo "Could not change directory to \"$WLROOTS_DIR\"."
    exit 1
fi

# Find all .h files in WLROOTS_DIR.
header_files=$(find . -type f ! -name 'cat.h' -a ! -name 'xdg-shell-protocol.h' -a -name "*.h")

for file in $header_files; do
    echo "Processing file: $file"

    # Find all occurrences of "} events;" with any indentation.
    events_lines=$(grep -n "[[:space:]]*} events;" "$file" | cut -d: -f1)

    for event_line in $events_lines; do
        echo "Found events ending at line $event_line"

        # Find the containing struct (search backwards for the last "struct " line without indentation).
        containing_struct=$(head -n "$event_line" "$file" | grep -n "^struct " | tail -1)

        if [ -z "$containing_struct" ]; then
            echo "Error: Could not find containing struct for events at line $event_line"
            exit 1
        fi

        struct_line_num=$(echo "$containing_struct" | cut -d: -f1)
        struct_def=$(echo "$containing_struct" | cut -d: -f2-)
        struct_name=$(echo "$struct_def" | sed -E 's/struct ([^ {]+).*/\1/')

        echo "Found containing struct '$struct_name' at line $struct_line_num"

        # Find the events struct start by searching backwards from the events end
        # to find a line with "struct {" at appropriate indentation.
        events_struct_start=""

        # First, get the line where events struct ends to determine indentation.
        events_end_line=$(sed -n "${event_line}p" "$file")
        indent=$(echo "$events_end_line" | sed 's/}.*//' | tr -d '\n')

        # Now search backwards for "struct {" with the same indentation.
        for ((i=event_line-1; i>struct_line_num; i--)); do
            line=$(sed -n "${i}p" "$file")
            if [[ "$line" == "$indent"*"struct {"* ]]; then
                events_struct_start=$i
                break
            fi
        done

        if [ -z "$events_struct_start" ]; then
            echo "Error: Could not find the start of events struct for line $event_line"
            exit 1
        fi

        echo "Events struct starts at line $events_struct_start and ends at line $event_line"

        # Extract all wl_signal lines within the events struct.
        wl_signal_lines=""
        for ((i=events_struct_start+1; i<event_line; i++)); do
            line=$(sed -n "${i}p" "$file")
            if [[ "$line" == *"struct wl_signal "* ]]; then
                wl_signal_lines="$wl_signal_lines $i"
            fi
        done

        if [ -z "$wl_signal_lines" ]; then
            echo "Warning: No wl_signal members found in events struct"
            continue
        fi

        for signal_line in $wl_signal_lines; do
            # Extract the signal name.
            signal_line_content=$(sed -n "${signal_line}p" "$file")
            signal_name=$(echo "$signal_line_content" | sed -E 's/.*struct wl_signal ([^;]+);.*/\1/')

            echo "Processing signal: $signal_name at line $signal_line"

            # Modify the signal name to add a '2' at the end.
            new_signal_name="${signal_name}2"
            sed -i "${signal_line}s/struct wl_signal ${signal_name}/struct wl_signal ${new_signal_name}/" "$file"

            echo "Modified signal $signal_name to $new_signal_name in $file"

            # Run the compiler and process errors.
            stop_processing=false
            while [ "$stop_processing" = false ]; do
                echo "Running compiler..."
                compiler_output=$(ninja -C "$WLROOTS_BUILDDIR" -j $CPUS -k 0 2>&1)

                # Check if compilation succeeded.
                if [ $? -eq 0 ]; then
                    echo "Compilation succeeded with no errors"
                    break
                fi

                # Look for errors related to our renamed signal;
                errors=$(echo "$compiler_output" | grep -F 'has no member named ‘'$signal_name'’')

                if [ -z "$errors" ]; then
					# No more errors found related to this signal.
					echo "No more errors found for $signal_name."
					break
                else
					# Check for wl_signal_emit_mutable lines.
					while IFS= read -r error_line; do
						error_file=$(echo "$error_line" | sed -E 's/^([^:]+):.*/\1/')
						error_line_num=$(echo "$error_line" | sed -E 's/^[^:]+:([0-9]+):.*/\1/')

						if [ -f "$error_file" ]; then
							line_content=$(sed -n "${error_line_num}p" "$error_file")
							echo "Considering line_content \"$line_content\""
							if [[ "$line_content" == *"wl_signal_emit_mutable"* ]]; then
								# Check if the last argument is not 'data'.
								if [[ "$line_content" != *", data)"* ]]; then
									echo "Found wl_signal_emit_mutable with non-data argument:"
									echo "$error_file:$error_line_num: $line_content"
									echo "Struct: $struct_name, Signal: $signal_name, Location: $error_file:$error_line_num"
									echo "$struct_name, $signal_name, $error_file:$error_line_num" >> $OUTPUT_FILE
									stop_processing=true
									break
								fi
							fi
						fi
					done <<< "$errors"

					if [ "$stop_processing" = true ]; then
						break
					fi

                    # Process each error related to the member name.
                    while IFS= read -r error; do
                        error_file=$(echo "$error" | sed -E 's/^([^:]+):.*/\1/')
                        error_line_num=$(echo "$error" | sed -E 's/^[^:]+:([0-9]+):.*/\1/')

                        echo "Fixing error in $error_file:$error_line_num"

                        # Replace signal name in the error file.
                        sed -i "${error_line_num}s/events\.${signal_name}/events\.${new_signal_name}/" "$error_file"
                    done <<< "$errors"
                fi
            done

            # Reset changes with git.
            echo "Resetting changes with git restore"
            git restore .
        done
    done
done

echo "All files processed."

popd
