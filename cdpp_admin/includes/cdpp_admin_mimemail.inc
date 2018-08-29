<?php

/**
 * @file
 * Contains cdpp admin mimemail hooks.
 */

/**
 * Implements hook_preprocess_mimemail_message().
 *
 * Conditionally add a no wrapper add any css as necessary.
 */
function cdpp_admin_preprocess_mimemail_message(&$variables) {
  // Dynamic insert the extra css as required.
  $no_wrapper_email_keys = array(
    'cdpp-admin-media-subscribe',
  );
  // If the email is one to override.
  if (isset($variables['key']) && in_array($variables['key'], $no_wrapper_email_keys)) {
    // If there is css to insert.
    if (isset($variables['css']) && $variables['css']) {
      // If we have a body to manipulate.
      if (isset($variables['body'])) {
        // Not using filter_dom_load() which returns a DOMDocument Object,
        // as it adds extra doctype/html to the html.
        $dom = new DOMDocument();
        $dom->loadHTML($variables['body']);
        // Find the head tag.
        $head = $dom->getElementsByTagName('head')->item(0);
        if ($head) {
          $new_elm = $dom->createElement('style', $variables['css']);
          $head->appendChild($new_elm);
          $variables['body'] = $dom->saveHTML();
        }
      }
    }
    if (isset($variables['theme_hook_suggestions'])) {
      array_unshift($variables['theme_hook_suggestions'], 'mimemail_message__no_wrapper');
    }
  }
}
