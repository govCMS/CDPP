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
  return _cdpp_menu_tree_inline($variables);
}

function cdpp_menu_tree__menu_top_menu($variables) {
  return _cdpp_menu_tree_inline($variables);
}

function cdpp_menu_tree__menu_secondary_menu($variables) {
  return _cdpp_menu_tree_inline($variables);
}

function cdpp_menu_tree__menu_block__1($variables) {
  return _cdpp_menu_tree_no_class($variables);
}

function cdpp_menu_tree__menu_block__4($variables) {
  return _cdpp_menu_tree_no_class($variables);
}

function cdpp_menu_tree__menu_govcms_menu_block_sidebar($variables) {
  return _cdpp_menu_tree_no_class($variables);
}

function cdpp_menu_tree(&$variables) {
  return '<div class="nav-collapse"><ul class="menu list-group">' . $variables['tree'] . '</ul></div>'; // added the nav-collapse wrapper so you can hide the nav at small size
}

function cdpp_menu_link__menu_block__govcms_menu_block_sidebar($variables) {
  return _cdpp_menu_link_with_bootstrap($variables);
}

function cdpp_menu_link__menu_block__1($variables) {
  return _cdpp_menu_link_with_bootstrap($variables);
}

function cdpp_menu_link__menu_block__4($variables) {
  return _cdpp_menu_link_with_bootstrap($variables);
}


function cdpp_menu_link($variables) {
  $element = $variables['element'];
  $sub_menu = '';
  //var_dump($element);
  if ($element['#below']) {
    // Ad our own wrapper
    unset($element['#below']['#theme_wrappers']);
    $sub_menu = '<ul class="list-group">' . drupal_render($element['#below']) . '</ul>'; // removed flyout class in ul

    unset($element['#localized_options']['attributes']['class']); // removed flydown class
    unset($element['#localized_options']['attributes']['data-toggle']); // removed data toggler
    // Check if this element is nested within another
    if ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] > 1)) {
      //unset($element['#attributes']['class']); // removed flyout class
    } else {
      //unset($element['#attributes']['class']); // unset flyout class
      $element['#localized_options']['html'] = TRUE;
      $element['#title'] .= ''; // removed carat spans flyout
    }

    // Set dropdown trigger element to # to prevent inadvertent page loading with submenu click
    $element['#localized_options']['attributes']['data-target'] = '#'; // You could unset this too as its no longer necessary.
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Implements hook_preprocess_node().
 */
function cdpp_preprocess_node(&$variables) {

  // Slides get a special read more link.
  if ($variables['type'] == 'slide') {
    if (!empty($variables['field_read_more'][0]['url'])) {
      $variables['title_link'] = l($variables['title'], $variables['field_read_more'][0]['url']);
    }
    else {
      $variables['title_link'] = check_plain($variables['title']);
    }
  }
}

function _cdpp_menu_tree_inline($variables) {
  return '<ul class="list-inline">' . $variables['tree'] . '</ul>';
}

function _cdpp_menu_tree_no_class($variables) {
  return '<ul>' . $variables['tree'] . '</ul>';
}

function _cdpp_menu_link_with_bootstrap($variables) {
  $element = $variables['element'];
  $sub_menu = '';
  if ($element['#below']) {
    // Ad our own wrapper
    unset($element['#below']['#theme_wrappers']);
    $sub_menu = '<ul class="list-group">' . drupal_render($element['#below']) . '</ul>'; // removed flyout class in ul

    unset($element['#localized_options']['attributes']['class']); // removed flydown class
    unset($element['#localized_options']['attributes']['data-toggle']); // removed data toggler
    // Check if this element is nested within another
    if ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] > 1)) {
      //unset($element['#attributes']['class']); // removed flyout class
      $element['#attributes']['class'][] = 'list-group-item';
    } else {
      //unset($element['#attributes']['class']); // unset flyout class
      $element['#attributes']['class'][] = 'list-group-item';
      $element['#localized_options']['html'] = TRUE;
      $element['#title'] .= ''; // removed carat spans flyout
    }

    // Set dropdown trigger element to # to prevent inadvertent page loading with submenu click
    $element['#localized_options']['attributes']['data-target'] = '#'; // You could unset this too as its no longer necessary.
  }
  $element['#attributes']['class'][] = 'list-group-item';
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}
