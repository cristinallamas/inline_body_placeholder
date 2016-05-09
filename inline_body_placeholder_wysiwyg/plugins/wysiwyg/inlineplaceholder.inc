<?php
/**
 * @file
 * inline_body_placeholder Inline/Wysiwyg API integration.
 */

// Default placeholder to load if variable is not set.
define("INLINE_BODY_PLACEHOLDER_PLACEHOLDER", '<div class="inline-body-placeholder"></div>');
/**
 * Implements hook_wysiwyg_plugin().
 */
function inline_body_placeholder_inlineplaceholder_plugin() {
  $plugins['inlineplaceholder'] = array(
    'title'      => t('Inline Body Placeholders'),
    'icon file'  => 'inlineplaceholder.png',
    'icon title' => t('Insert inline ad slot placeholder'),
    'settings'   => array(
      'inlineWysiwygPattern' => variable_get('inline_body_placeholder_placeholder',INLINE_BODY_PLACEHOLDER_PLACEHOLDER),
    ),
  );
  return $plugins;
}
