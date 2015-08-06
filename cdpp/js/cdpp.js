var Drupal = Drupal || {};

(function($, Drupal){

    jQuery(document).ready(function($) {
        $('table').each(function() {
            $(this).addClass('table');
        });
    });


})(jQuery, Drupal);