<?php

/**
 * @file
 * template.php
 */


/**
 * Implement hook_js_alter()
 * Attempt to replace the system jQuery on non admin and non node admin pages with a newer version provided by the theme
 */

function cdpp_js_alter(&$javascript) {
  $node_admin_paths = array(
    'node/*/edit',
    'node/add',
    'node/add/*',
    'node/*/extend_review_date',
  );
  $replace_jquery = TRUE;
  if (path_is_admin(current_path())) {
    $replace_jquery = FALSE;
  } else {
    foreach ($node_admin_paths as $node_admin_path) {
      if (drupal_match_path(current_path(), $node_admin_path)) {
        $replace_jquery = FALSE;
      }
    }
  }
// Swap out jQuery to use an updated version of the library.
  if ($replace_jquery) {
    $javascript['misc/jquery.js']['data'] = '//code.jquery.com/jquery-2.1.4.min.js';
  }
}

function cdpp_menu_tree__menu_footer_sub_menu($variables) {
  return '<ul class="list-inline">' . $variables['tree'] . '</ul>';
}

function cdpp_menu_tree__menu_top_menu($variables) {
  return '<ul class="list-inline">' . $variables['tree'] . '</ul>';
}

function cdpp_menu_tree__menu_secondary_menu($variables) {
  return '<ul class="list-inline">' . $variables['tree'] . '</ul>';
}
