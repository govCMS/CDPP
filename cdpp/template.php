<?php

/**
 * @file
 * template.php
 */

// Include helper files.
include_once dirname(__FILE__) . '/includes/cdpp_webform_hooks.inc';
include_once dirname(__FILE__) . '/includes/cdpp_theme_hooks.inc';
include_once dirname(__FILE__) . '/includes/cdpp_govcms_text_resize_hooks.inc';

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
  // Swap out jquery.form.js to avoid handleError is not a function on webform
  // ajax.
  if (isset($javascript['misc/jquery.form.js'])) {
    $javascript['misc/jquery.form.js']['data'] = '//cdnjs.cloudflare.com/ajax/libs/jquery.form/4.2.2/jquery.form.min.js';
  }
  _cdpp_js_alter__text_resize($javascript);
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
  // Optionally, run node-type-specific preprocess functions, like
  // foo_preprocess_node__page().
  if (isset($variables['node']->type)) {
    $function = __FUNCTION__ . '__' . $variables['node']->type;
    if (function_exists($function)) {
      $function($variables);
    }
  }
}

/**
 * Implements hook_preprocess_block().
 */
function cdpp_preprocess_block(&$variables) {
  // fix the undefined title variable warning.
  if(!isset($variables['title'])) {
    $variables['title'] = '';
  }
}

/**
 * Implements hook_preprocess_region().
 */
function cdpp_preprocess_region(&$variables) {
   if(isset($variables['elements'])) {
    if(isset($variables['elements']['#region'])) {
      if($variables['elements']['#region'] == 'sidebar_first') {
        if(drupal_get_path_alias() == "crimes-we-prosecute" || drupal_get_path_alias() == "case-reports") {
          $variables['classes_array'][] = '-mobile-exclude';
        }
      }
    }
  }
}

/**
 * Implements hook_preprocess_entity().
 */
function cdpp_preprocess_entity(&$variables) {
  $paragraph_entity = $variables['elements']['#entity'];
  if (isset($variables['elements']['#entity']->bundle)) {
    $function = __FUNCTION__ . '__' . $variables['elements']['#entity']->bundle;
    if (function_exists($function)) {
      $function($variables,$paragraph_entity);
    }
  }

  // Use field_svg_icon to handle svg image.
  if(isset($variables['elements']['#entity']->bundle) && $paragraph_entity) {
    $field_icon = field_get_items('paragraphs_item',$paragraph_entity,'field_svg_icon');
    if ($field_icon) {
      $icon_markup = file_get_contents($field_icon[0]['uri']);
      $variables['icon_image'] = $icon_markup;
    }
  }
}

/**
 * Implements hook_preprocess_entity().
 */
function cdpp_preprocess_entity__homepage_information_bundle_top(&$variables,&$paragraph_entity) {
  $field_link_to = isset($paragraph_entity) ? field_get_items('paragraphs_item',$paragraph_entity,'field_link_to') : null;
  if ($field_link_to) {
    $variables['link_to'] = $field_link_to[0]['url'];
    $variables['link_to_title'] = $field_link_to[0]['title'];
  }
}
/**
 * Implements hook_preprocess_entity().
 */
function cdpp_preprocess_entity__standard_page_crime_bundle(&$variables,&$paragraph_entity) {
  $field_link_to = isset($paragraph_entity) ? field_get_items('paragraphs_item',$paragraph_entity,'field_link_to') : null;
  if ($field_link_to) {
    $variables['link_to'] = $field_link_to[0]['url'];
  }
}

/**
 * Implements hook_preprocess_entity().
 */
function cdpp_preprocess_entity__standard_page_case_bundle(&$variables,&$paragraph_entity) {
  $field_category = isset($paragraph_entity) ? field_get_items('paragraphs_item',$paragraph_entity,'field_category') : null;
  if ($field_category && ($view = views_get_view('case_reports_views')) && $view->set_display('page_1')) {
    $field_category_tid = $field_category[0]['tid'];
    $view_url = $view->get_url();
    $variables['link_to'] = url($view_url, array('query' => array('field_category_tid' => $field_category_tid)));
  }
}

/**
 * Implements hook_preprocess_field().
 */
function cdpp_preprocess_field(&$variables) {
  if($variables['element']['#field_name'] == 'field_bean_image'){
    foreach($variables['items'] as $key => $item){
      $variables['items'][ $key ]['#item']['attributes']['class'][] = 'grayscale grayscale-fade';
    }
  }
}

/**
 * Implements hook_preprocess_page().
 */
function cdpp_preprocess_page(&$variables) {
  // Get SVG Sprites.
  $variables['svg_sprites'] = '';
  $svg_sprite_location = DRUPAL_ROOT . '/' . path_to_theme() . '/images/svg-sprite.svg';
  if (file_exists($svg_sprite_location)) {
    $variables['svg_sprites'] = file_get_contents($svg_sprite_location);
  }
  // Add information about the number of sidebars.
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-sm-6"';
  }
  elseif (!empty($variables['page']['sidebar_first']) || !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-sm-9"';
  }
  elseif(drupal_is_front_page()) {
    $variables['content_column_class'] = ' class="col-sm-12"';
  } else {
    $variables['content_column_class'] = ' class="col-sm-9"';
  }

  if(bootstrap_setting('fluid_container') === 1) {
    $variables['container_class'] = 'container-fluid';
  }
  else {
    $variables['container_class'] = 'container';
  }

  // Primary nav.
  $variables['primary_nav'] = FALSE;
  if ($variables['main_menu']) {
    // Build links.
    $variables['primary_nav'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    // Provide default theme wrapper function.
    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__primary');
  }

  // Secondary nav.
  $variables['secondary_nav'] = FALSE;
  if ($variables['secondary_menu']) {
    // Build links.
    $variables['secondary_nav'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
    // Provide default theme wrapper function.
    $variables['secondary_nav']['#theme_wrappers'] = array('menu_tree__secondary');
  }

  $variables['navbar_classes_array'] = array('navbar');

  if (bootstrap_setting('navbar_position') !== '') {
    $variables['navbar_classes_array'][] = 'navbar-' . bootstrap_setting('navbar_position');
  }
  elseif(bootstrap_setting('fluid_container') === 1) {
    $variables['navbar_classes_array'][] = 'container-fluid';
  }
  else {
    $variables['navbar_classes_array'][] = 'container';
  }
  if (bootstrap_setting('navbar_inverse')) {
    $variables['navbar_classes_array'][] = 'navbar-inverse';
  }
  else {
    $variables['navbar_classes_array'][] = 'navbar-default';
  }

  if (drupal_is_front_page()) {
    if (isset($variables['page']['content']['bean_homepage-exhibition-block-right']) &&
        isset($variables['page']['content']['bean_homepage-exhibition-block-left'])) {
        if($variables['page']['content']['bean_homepage-exhibition-block-right']['#weight'] <
            $variables['page']['content']['bean_homepage-exhibition-block-left']['#weight']) {
          $variables['page']['content']['bean_homepage-exhibition-block-right']['#prefix'] = '<div class="cdpp-homepage-exhibition-block-wrapper">';
          $variables['page']['content']['bean_homepage-exhibition-block-left']['#suffix'] = '</div>';
        }
    }

    if(isset($variables['page']['content']['bean_homepage-information-block-base']) &&
      isset($variables['page']['content']['bean_homepage-subscription-block'])) {
        if ($variables['page']['content']['bean_homepage-information-block-base']['#weight'] <
            $variables['page']['content']['bean_homepage-subscription-block']['#weight']) {
          $variables['page']['content']['bean_homepage-information-block-base']['#prefix'] = '<div class="cdpp-homepage-base"><div class="cdpp-homepage-base-inner-wrapper">';
          $variables['page']['content']['bean_homepage-subscription-block']['#suffix'] = '</div></div>';
        }
    }
    elseif(isset($variables['page']['content']['bean_homepage-information-block-base'])) {
      $variables['page']['content']['bean_homepage-information-block-base']['#prefix'] = '<div class="cdpp-homepage-base"><div class="cdpp-homepage-base-inner-wrapper">';
      $variables['page']['content']['bean_homepage-information-block-base']['#suffix'] = '</div></div>';
    }
  }
}

function _cdpp_menu_tree_inline($variables) {
  //$variables['tree'] = 's';//s_string($variables['tree']) ? $variables['tree'] : 'why an i an array?';
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
