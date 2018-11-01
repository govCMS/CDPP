<?php

/**
 * @file
 * Default theme implementation for a single paragraph item.
 *
 * Available variables:
 * - $content: An array of content items. Use render($content) to print them
 *   all, or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. By default the following classes are available, where
 *   the parts enclosed by {} are replaced by the appropriate values:
 *   - entity
 *   - entity-paragraphs-item
 *   - paragraphs-item-{bundle}
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened into
 *   a string within the variable $classes.
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */
?>
<div class="entity entity-paragraphs-item paragraphs-item-homepage-information-bundle-top" about="" typeof="">
  <div class="content">
    <div class="field field-name-field-link-to field-type-link-field field-label-hidden">
        <a href="<?php print $link_to; ?>" target="_blank">
          <span> <?php print $link_to_title; ?> </span>
          <svg class="arrow-icon"><use xlink:href="#arrow-right"></use></svg>
      </a>
    </div>
    <div class="field field-name-field-icon field-type-file field-label-hidden">
      <div class="field-items">
        <div class="field-item even">
          <?php print($icon_image); ?>
        </div>
      </div>
    </div>
    <?php print render($content['field_body']); ?>
  </div>
</div>
