<?php

/**
 * @file
 * template.php
 */

// Include helper files.
include_once dirname(__FILE__) . '/includes/cdpp_admin_theme_disable.inc';
include_once dirname(__FILE__) . '/includes/cdpp_admin_theme_hooks.inc';
include_once dirname(__FILE__) . '/includes/cdpp_admin_media_subscribe_notification.inc';
include_once dirname(__FILE__) . '/includes/cdpp_admin_menu_hooks.inc';
include_once dirname(__FILE__) . '/includes/cdpp_admin_mimemail.inc';
include_once dirname(__FILE__) . '/includes/cdpp_admin_variables.inc';
include_once dirname(__FILE__) . '/includes/cdpp_admin_webform_admin.inc';
include_once dirname(__FILE__) . '/includes/cdpp_admin_webform_admin_media_subscribe.inc';

/**
 * Implements hook_form_BASE_FORM_ID_alter().
 *
 * Customise and react to node forms.
 */
function cdpp_admin_form_node_form_alter(&$form, &$form_state, $form_id) {
  cdpp_admin_form_node_form_alter__func__media_subscribe($form, $form_state, $form_id);
}
