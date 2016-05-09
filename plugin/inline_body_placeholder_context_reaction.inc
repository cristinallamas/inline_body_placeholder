<?php

/**
 * @file
 * Context reaction plugin for Lazy Loading Ad Slots.
 */

/**
 * Class LazyAdSlotContextReaction
 */
class InlineBodyPlaceholderContextReaction extends context_reaction {
  public $lazyadslot_tags = [];

  // Form options.
  function options_form($context) {
    $form = [];
    $values = [];

    // Get existing values for this form.
    $settings = $this->fetch_from_context($context);
    if (!empty($settings['values'])) {
      $values = $settings['values'];
    }

    // Get a list of all the tags.
    $tags = dfp_tag_load_all();
    if (empty($tags)) {
      return [
        '#markup' => t('Before you can use this reaction you need to <a href="/admin/structure/dfp_ads/add">Add a new DFP ad</a>.')
      ];
    }

    $options = [];
    foreach ($tags as $tag) {
      $options[$tag->machinename] = $tag->slot;
    }


    $form['inline_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Global Inline Settings'),
      '#description' => t('The following settings control the automated placement of inline ad slots.'),
      '#group' => 'settings',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#attached' => array(
        'js' => array(
          'vertical-tabs' => drupal_get_path('module', 'dennis_dfp_inline') . '/js/admin/dennis_dfp_inline.admin.js',
        ),
      ),
    );
    $form['inline_settings']['inline_selector'] = array(
      '#type' => 'textfield',
      '#title' => t('Field content selector'),
      '#default_value' => variable_get('inline_selector', DENNIS_DFP_INLINE_SELECTOR),
      '#description' => t('The DOM selector which translates to a single element with children as paragraphs or other blocks (media images, headings, etc.). This will be used for analysis and the placement of inline ad slots.'),
    );
    $form['inline_settings']['inline_placeholder'] = array(
      '#type' => 'textfield',
      '#title' => t('Inline ad placeholder'),
      '#default_value' => variable_get('inline_placeholder', DENNIS_DFP_INLINE_PLACEHOLDER),
      '#description' => t('The pattern to use for manually inserted inline ad slot placeholders.'),
    );
    $form['inline_settings']['inline_first_position'] = array(
      '#type' => 'select',
      '#title' => t('Automatically insert the first ad tag'),
      '#options' => array(
        1 => t('first'),
        2 => t('second'),
        3 => t('third'),
        4 => t('fourth'),
        5 => t('fifth'),
      ),
      '#field_prefix' => t('Before the'),
      '#field_suffix' => t('paragraph or other block level element (media file, etc.)'),
      '#default_value' => variable_get('inline_first_position', DENNIS_DFP_INLINE_FIRST_POSITION),
      '#description' => t('The first ad slot should appear BEFORE the configured element. Please note that this only applies if there are no manually added placeholders in the content.'),
    );
    $form['inline_settings']['inline_min_distance'] = array(
      '#type' => 'select',
      '#title' => t('Minimum distance between subsequent ad slots in content'),
      '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)),
      '#field_suffix' => t('paragraph or other block level element (media file, etc.)'),
      '#default_value' => variable_get('inline_min_distance', DENNIS_DFP_INLINE_MIN_DISTANCE),
      '#description' => t('The minimum distance measured in paragraphs between the automated ad slot placements in content. If content is shorter then ads will be added to the end of the content.'),
    );
    return $form;
  }

  /**
   * Options form submit handler.
   *
   * As the context do not provide any validate method, will display a error
   * after all the values are submitted already.
   * (#required in the form element will break module non related contexts.)
   */
  function options_form_submit($values) {
    if (empty($values['values']['ad_placement']) && empty($values['values']['disable_dom_rendering'])) {
      form_set_error('reactions[plugins][lazyadslot][values][ad_placement]', t('Please provide at least one selector.'));
    }
    return $values;
  }

  // Execute context.
  function execute() {
    $i = 0;
    foreach ($this->get_contexts() as $context) {
      if (!isset($context->reactions['lazyadslot'])) {
        continue;
      }

      foreach ($context->reactions['lazyadslot'] as $values) {
        if (!empty($values)) {
          $this->lazyadslot_tags['adslot_' . ++$i] = $values;
        }
      }
    }
  }
}
