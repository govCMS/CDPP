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
    <div class="content"<?php print $content_attributes; ?>>
      <div>
        <h3> <?php print $title_link; ?> </h3>
        <div class="node-slide__tag" role="paragraph"><span class="sr-only">Type: </span> <?php print render($content['field_title']); ?> </div>

        <?php
        hide($content['links']);
        hide($content['field_title']);
        print render($content);
        ?>
      </div>



      <div class="carouselControls">
        <h4 class="sr-only">Carousel controls</h4>

        <ol class="carouselControls__pager carouselPager">
          <?php if(isset($view)): ?>
            <?php foreach (array_keys($view->result) as $row_index): ?>
              <?php $row_index_attr = ($view->row_index == $row_index) ? ' aria-current="true"' : ''; ?>
              <?php $slide_index = $row_index + 1; ?>
              <li class="carouselPager__item">
                <button class="carouselPager__btn" type="button" title="View slide <?php print $slide_index; ?>"<?php print $row_index_attr; ?>><?php print $row_index; ?></button>
            </li>
            <?php endforeach; ?>
          <?php endif; ?>
        </ol>

        <button class="pauseBtn carouselControls__pauseBtn" type="button" title="Toggle carousel autoplay">
          <span class="pauseBtn__text sr-only">Pause</span>
          <i class="pauseBtn__icon" aria-hidden="true"></i>
        </button>
      </div>
    </div>

    <?php print render($content['links']); ?>
  </div>
</div>


