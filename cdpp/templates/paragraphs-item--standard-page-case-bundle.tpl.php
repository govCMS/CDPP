<div class="entity entity-paragraphs-item paragraphs-item-standard-page-content-bundle" about="" typeof="">
  <a href="<?php print isset($link_to) ? $link_to : ''; ?>">
    <div class="content">
      <div class="field field-name-field-icon field-type-file field-label-hidden">
        <div class="field-items">
          <div class="field-item even">
            <?php print($icon_image); ?>
          </div>
        </div>
      </div>
      <?php print render($content['field_title']); ?>
      <svg class="arrow-icon"><use xlink:href="#arrow-right"></use></svg>
    </div>
  </a>
</div>
