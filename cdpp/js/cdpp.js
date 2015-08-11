var Drupal = Drupal || {};

(function($, Drupal){

    jQuery(document).ready(function($) {
        $('table').each(function() {
            $(this).addClass('table');
        });

        if(jQuery("ul.rslides_tabs").length) {
            //Put in a previous and next button
            jQuery("ul.rslides_tabs").prepend("<li><a href='#' id='prevSlides' title='Select to go to previous' class='skip'>&#60;</a></li>");
            jQuery("ul.rslides_tabs").append("<li><a href='#' id='nextSlides' title='Select to go to next' class='skip'>&#62;</a></li>");

            //jQuery("ul.rslides_tabs").append("<li><a href='#' id='stopStartSlide' title='Select to Pause' class='started skip'>&#9614;&#9614;</a></li>");

            //Slideshow exists... lets change them every 7 seconds
            var interval = setInterval(function() {
                if(jQuery("li.rslides_here").is(':nth-last-child(2)')) {
                    jQuery("a.rslides1_s1").click();
                } else {
                    jQuery("li.rslides_here").next().find("a").not('.skip').click();
                }

            }, 7000);

            jQuery("ul.rslides_tabs > li").each(function() {
                jQuery(this).find("a").each(function() {
                    jQuery(this).click(function(e) {
                        if(e.hasOwnProperty('originalEvent')) {
                            clearInterval(interval);
                        }
                    });
                });
            });

            jQuery('#prevSlides').click(function() {
                clearInterval(interval);
                if(jQuery("li.rslides_here").is(':nth-child(2)')) {
                    jQuery("li.rslides_here").next().find("a").not('.skip').click();
                } else {
                    jQuery("a.rslides1_s1").click();
                }
            });

            jQuery('#nextSlides').click(function() {
                clearInterval(interval);
                if(jQuery("li.rslides_here").is(':nth-last-child(2)')) {
                    jQuery("a.rslides1_s1").click();
                } else {
                    jQuery("li.rslides_here").next().find("a").not('.skip').click();
                }
            });

            jQuery("#stopStartSlide").click(function() {
                if(jQuery(this).hasClass("started")) {
                    clearInterval(interval);
                    jQuery(this).removeClass("started");
                    jQuery(this).addClass("stopped");
                    jQuery(this).html("&#9658;");
                    jQuery(this).attr("title", "Select to Play");
                } else {
                    interval = setInterval(function() {
                        if(jQuery("li.rslides_here").is(':nth-last-child(2)')) {
                            jQuery("a.rslides1_s1").click();
                        } else {
                            jQuery("li.rslides_here").next().find("a").click();
                        }

                    }, 7000);
                    jQuery(this).addClass("started");
                    jQuery(this).removeClass("stopped");
                    jQuery(this).html("&#9614;&#9614;");
                    jQuery(this).attr("title", "Select to Pause");
                }
            });
        }

        jQuery('.custom-grayscale').each(function() {
            jQuery(this).hover(
                function() {jQuery(this).addClass('grayscale')},
                function() {jQuery(this).removeClass('grayscale')}
            );
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


(function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.responsiveSlides = {
        attach: function(context, settings) {

            $(".view-slideshow ul:not(.contextual-links)").responsiveSlides({
                "auto": false,
                "pager": true,         // Boolean: Show pager, true or false
                "pauseButton": false   // Boolean: Create Pause Button
                // "pauseButton": true   // Boolean: Create Pause Button
            });

        }
    };


})(jQuery, Drupal, this, this.document);
