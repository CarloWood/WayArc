#include "sys.h"
#include "Scene.h"
#include <cstdlib>
#include "debug.h"

namespace wlr {

namespace {

bool env_parse_bool(char const* option)
{
  char const* env = std::getenv(option);
  Dout(dc::notice(env), "Loading " << option << " option: " << env);

  if (!env || strcmp(env, "0") == 0)
    return false;
  else if (strcmp(env, "1") == 0)
    return true;

  Dout(dc::warning, "Unknown " << option << " option: " << env);
  return false;
}

size_t env_parse_switch(char const* option, char const* const* switches)
{
  char const* env = std::getenv(option);
  Dout(dc::notice(env), "Loading " << option << " option: " << env);
  if (!env)
    return 0;

  for (ssize_t i = 0; switches[i]; ++i)
    if (strcmp(env, switches[i]) == 0)
      return i;

  Dout(dc::warning, "Unknown " << option << " option: " << env);
  return 0;
}

} // namespace

void Scene::Node::init(Tree* parent)
{
  type_ = NODE_TREE;
  parent_ = parent;
  wl_list_init(&link_);
  enabled_ = true;
  wl_signal_init(&events_.destroy);
  wlr_addon_set_init(&addons_);
  pixman_region32_init(&visible_);

  if (parent_)
    wl_list_insert(parent_->children_.prev, &link_);
}

void Scene::Tree::init(Tree* parent)
{
  node_.init(parent);

  wl_list_init(&children_);
}

void Scene::init()
{
  DoutEntering(dc::notice, "Scene::init()");

  tree.init(nullptr);

  wl_list_init(&outputs);
  wl_list_init(&linux_dmabuf_v1_destroy.link);
  wl_list_init(&gamma_control_manager_v1_destroy.link);
  wl_list_init(&gamma_control_manager_v1_set_gamma.link);

  char const* const debug_damage_options[] = {"none", "rerender", "highlight", nullptr};

  switch (env_parse_switch("WLR_SCENE_DEBUG_DAMAGE", debug_damage_options))
  {
    case DEBUG_DAMAGE_RERENDER:
      debug_damage_option = DEBUG_DAMAGE_RERENDER;
      break;
    case DEBUG_DAMAGE_HIGHLIGHT:
      debug_damage_option = DEBUG_DAMAGE_HIGHLIGHT;
      break;
    default:
      debug_damage_option = DEBUG_DAMAGE_NONE;
      break;
  }
  direct_scanout = !env_parse_bool("WLR_SCENE_DISABLE_DIRECT_SCANOUT");
  calculate_visibility = !env_parse_bool("WLR_SCENE_DISABLE_VISIBILITY");
  highlight_transparent_region = env_parse_bool("WLR_SCENE_HIGHLIGHT_TRANSPARENT_REGION");
}

} // namespace wlr
