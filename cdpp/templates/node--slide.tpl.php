<?php

/**
 * @file
 * Markup for slide node-type.
 */

?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <div class="group-left col-md-8 col-sm-12">
    <?php print render($content['field_slide_image']) ?>
  </div>
  <div class="group-right col-md-4 col-sm-12">
    <h2 class="node__title node-title slide__title"><?php print $title_link; ?></h2>

    <div class="content"<?php print $content_attributes; ?>>
      <?php
      hide($content['links']);
      print render($content);
      ?>
    </div>

    <?php print render($content['links']); ?>

  </div>
</div>
