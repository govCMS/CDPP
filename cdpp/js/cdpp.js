var Drupal = Drupal || {};

(function($, Drupal){

    jQuery(document).ready(function($) {
        $('table').each(function() {
            $(this).addClass('table');
        });
    });

    equalheight = function(container){

        var currentTallest = 0,
            currentRowStart = 0,
            rowDivs = new Array(),
            el,
            topPosition = 0;
        jQuery(container).each(function() {

            el = jQuery(this);
            jQuery(el).height('auto')
            topPostion = el.position().top;

            if (currentRowStart != topPostion) {
                for (currentDiv = 0 ; currentDiv < rowDivs.length ; currentDiv++) {
                    rowDivs[currentDiv].height(currentTallest);
                }
                rowDivs.length = 0; // empty the array
                currentRowStart = topPostion;
                currentTallest = el.height();
                rowDivs.push(el);
            } else {
                rowDivs.push(el);
                currentTallest = (currentTallest < el.height()) ? (el.height()) : (currentTallest);
            }
            for (currentDiv = 0 ; currentDiv < rowDivs.length ; currentDiv++) {
                rowDivs[currentDiv].height(currentTallest);
            }
        });
    }

    jQuery(window).load(function() {
        if(jQuery(window).width() >= 650){
            equalheight('body.front div.panel-col-bottom div.panel-pane div.panel-default div.panel-body > div.row');
            equalheight('body.front div.panel-col-top div.jumbotron');
        }
    });

})(jQuery, Drupal);