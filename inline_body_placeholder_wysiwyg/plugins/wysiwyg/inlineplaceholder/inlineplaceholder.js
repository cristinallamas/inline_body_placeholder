/**
 * @file
 * Lazyadslot Inline WYSIWYG plugin
 */
(function() {

  Drupal.wysiwyg.plugins.inlineplaceholder = {

    /**
     * Execute the button.
     */
    invoke: function (data, settings, instanceId) {
      var content = (data.format === 'html') ? this._getPlaceholder(settings) : settings.inlineWysiwygPattern;

      Drupal.wysiwyg.instances[instanceId].insert(content);
    },

    /**
     * Replace all placeholders with images.
     */
    attach: function (content, settings) {
      var inlineWysiwygPattern = settings.inlineWysiwygPattern;
      var placeholder = this._getPlaceholder(settings);
      // Some WYSIWYGs (CKEditor) will strip the slash from single tags:
      // <foo /> becomes <foo>
      var inlineAdSlot = inlineWysiwygPattern.replace(/\/>/, '/?>').replace(/ /g, ' ?');

      // Remove unnecessary paragraph.
      var pattern = new RegExp('<p>' + inlineAdSlot + '</p>', 'ig');
      content = content.replace(pattern, placeholder);
      // Move breaks starting at the beginning of paragraphs to before them.
      pattern = new RegExp('<p>' + inlineAdSlot + '(<[^p])', 'ig');
      content = content.replace(pattern, placeholder + '<p>$1');
      // Move breaks starting at the end of to after the paragraphs.
      pattern = new RegExp('([^p]>)' + inlineAdSlot + '<\/p>', 'ig');
      content = content.replace(pattern, '$1</p>' + placeholder);
      // Other breaks.
      inlineAdSlot =  new RegExp(inlineAdSlot, 'g');
      content = content.replace(inlineAdSlot, placeholder);

      return content;
    },

    /**
     * Replace images with inlineWysiwygPattern tags in content upon detaching editor.
     */
    detach: function(content, settings) {
      var inlineWysiwygPattern = settings.inlineWysiwygPattern;
      // Some WYSIWYGs (CKEditor) will strip the slash from single tags:
      // <foo /> becomes <foo>
      var inlineAdSlot = inlineWysiwygPattern.replace(/\/>/, '/?>').replace(/ /g, ' ?');
      // Replace (duplicate) placeholders within p tags with a single break.
      // var newContent = content.replace(/\s*<p[^>]*?>(?:\s*<img(?:\s*\w+=['"][^'"]*?['"]\s*)*?\s*class=['"][^'"]*?dfp-inline-placeholder[^'"]*?['"]\s*(?:\s*\w+=['"][^'"]*?['"]\s*)*?(?:\/)?>\s*)+<\/p>\s*/ig, dfpInlinePattern);

      // Replace all other placeholders.
      var newContent = content.replace(/<img(?:\s*\w+=['"][^'"]*?['"]\s*)*?\s*class=['"][^'"]*?inline-body-placeholder[^'"]*?['"]\s*(?:\s*\w+=['"][^'"]*?['"]\s*)*?(?:\/)?>/ig, inlineWysiwygPattern);
      // Fix paragraphs opening just before breaks.
      var pattern = new RegExp('(?:' + inlineAdSlot + ')*(<p[^>]*?>\\s*)' + inlineAdSlot, 'ig');
      newContent = newContent.replace(pattern, inlineWysiwygPattern + '$1');
      // Remove duplicate breaks and any preceding whitespaces.
      pattern = new RegExp('(?:\\s*' + inlineAdSlot + '){2,}' + inlineAdSlot, 'ig');
      newContent = newContent.replace(pattern, inlineWysiwygPattern);
      // Fix paragraphs ending after breaks.
      pattern = new RegExp(inlineAdSlot + '(\\s*<\/p>)(?:' + inlineAdSlot + ')*', 'ig');
      newContent = newContent.replace(pattern, '$1' + inlineWysiwygPattern);
      // Remove duplicate breaks with trailing whitespaces.
      pattern = new RegExp('(?:' + inlineAdSlot + '\\s*){2,}', 'ig');
      newContent = newContent.replace(pattern, inlineWysiwygPattern);

      return newContent;
    },

    /**
     * Helper function to return a HTML placeholder.
     */
    _getPlaceholder: function (settings) {
      return '<img src="' + settings.path + '/images/placeholder.png" alt="&lt;--#lazyadslotWysiwyg#--&gt;" title="&lt;--#lazyadslotWysiwyg#--&gt;" class="inline-body-placeholder drupal-content" />';
    }
  };

}(jQuery));
