<?php

/**
 * @file
 * Contains cdpp webform hooks.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function cdpp_admin_form_webform_node_form_alter(&$form, &$form_state, $form_id) {
  if (isset($form_state['node']->machine_name)) {
    $machine_name = $form_state['node']->machine_name;
    $function_name = __FUNCTION__ . '__func__admin__' . $machine_name;
    if (function_exists($function_name)) {
      $function_name($form, $form_state, $form_id);
    }
  }
}
