#pragma once

extern "C" {
#include <wlr/types/wlr_linux_dmabuf_v1.h>
#include <wlr/types/wlr_gamma_control_v1.h>
#include <wlr/util/addon.h>
} // extern "C"

#include <wayland-util.h>
#include <wayland-server-core.h>
#include <pixman.h>

namespace wlr {

class Scene
{
 public:
#ifdef CWDEBUG
  // Copy of wlr_scene_debug_damage_option.
  enum scene_debug_damage_option_t
  {
    DEBUG_DAMAGE_NONE,
    DEBUG_DAMAGE_RERENDER,
    DEBUG_DAMAGE_HIGHLIGHT
  };
#endif

  enum node_type_t
  {
    NODE_TREE,
    NODE_RECT,
    NODE_BUFFER
  };

  struct Tree;

  // A Node is an object in the scene.
  struct Node
  {
    node_type_t type_;
    Tree* parent_;
    struct wl_list link_; // wlr_scene_tree.children
    bool enabled_;
    int x_{}, y_{};     // relative to parent_
    struct { struct wl_signal destroy; } events_;
    void* data_ = nullptr;
    struct wlr_addon_set addons_;

   private:
    pixman_region32_t visible_;

   public:
    void init(Tree* parent);
  };

  // A sub-tree in the scene-graph.
  struct Tree
  {
    Node node_;
    struct wl_list children_;

    void init(Tree* parent);
  };

 private:
  Tree tree;

  struct wl_list outputs; // wlr_scene_output.link

  // May be nullptr.
  struct wlr_linux_dmabuf_v1* linux_dmabuf_v1 = nullptr;
  struct wlr_gamma_control_manager_v1* gamma_control_manager_v1 = nullptr;

 private:
  struct wl_listener linux_dmabuf_v1_destroy{};
  struct wl_listener gamma_control_manager_v1_destroy{};
  struct wl_listener gamma_control_manager_v1_set_gamma{};

#ifdef CWDEBUG
  scene_debug_damage_option_t debug_damage_option;
#endif
  bool direct_scanout;
  bool calculate_visibility;
  bool highlight_transparent_region;

 public:
  void init();
};

} // namespace wlr

