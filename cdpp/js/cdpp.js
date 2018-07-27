var Drupal = Drupal || {};

/**
 * @file
 * This file controls accessibility functions on the theme layer.
 * Based on: http://www.acfonline.org.au/sites/all/themes/acf/js/acf.accessibility.js
 */

(function ($) {

    localStorage.size = localStorage.size || 'normal';

    /*
     * @function govAccessibilityTextSize
     * Controls the text resizer
     */
    Drupal.behaviors.govAccessibilityTextSize = {
      attach: function (context, settings) {

          function enlarge(){
            $('html').addClass('large-fonts');
            $('.font-large').attr('aria-selected',true);
            $('.font-small').attr('aria-selected',false);
            localStorage.size = 'large';
          }

          function shrink(){
            $('html').removeClass('large-fonts');
            $('.font-large').attr('aria-selected',false);
            $('.font-small').attr('aria-selected',true);
            localStorage.size = 'normal';
          }

          if (localStorage.size === 'large') {
            enlarge();
          }

          $('.font-large, .font-small').on('click keypress',function(e) {
            if (e.type === 'click' || e.type === 'keypress' && e.key === 'Enter' ) {
              var isLargeBtn = $(this).hasClass('font-large');
              e.preventDefault();

              if (isLargeBtn) {
                enlarge();
              } else {
                shrink();
              }
            }
          });
        }
    };

}(jQuery));


(function($, Drupal){

    jQuery(document).ready(function($) {

        // Back to top button on report
        if(jQuery('.annual-report_table-of-content').length > 0){
          jQuery('body').append('<div class="annual-report_back-to-top">Back</div>');
          window.onscroll = function() {
            if (document.body.scrollTop > 20 || document.documentElement.scrollTop > 20) {
                jQuery('.annual-report_back-to-top').addClass('active');
            } else {
                jQuery('.annual-report_back-to-top').removeClass('active');
            }
          };
          jQuery('.annual-report_back-to-top').on("click",function() {jQuery('html, body').animate({ scrollTop: 0 }, 'slow', function () {});});
        }
        
        //bacl to top button on report new element
        if(jQuery('.report-new-element_table-of-content').length > 0){
          jQuery('body').append('<div class="report-new-element_back-to-top">Back</div>');
          window.onscroll = function() {
            if (document.body.scrollTop > 20 || document.documentElement.scrollTop > 20) {
                jQuery('.report-new-element_back-to-top').addClass('active');
            } else {
                jQuery('.report-new-element_back-to-top').removeClass('active');
            }
          };
          jQuery('.report-new-element_back-to-top').on("click",function() {jQuery('html, body').animate({ scrollTop: 0 }, 'slow', function () {});});
        }


        $('table').each(function() {
            $(this).addClass('table');
        });

        if(jQuery("ul.rslides_tabs").length) {
            //Put in a previous and next button
            jQuery("ul.rslides_tabs").prepend("<li><a href='#' id='prevSlides' title='Select to go to previous' class='skip'><i class='icon-angle-circled-left'></i></a></li>");
            jQuery("ul.rslides_tabs").append("<li><a href='#' id='nextSlides' title='Select to go to next' class='skip'><i class='icon-angle-circled-right'></i></a></li>");

            //jQuery("ul.rslides_tabs").append("<li><a href='#' id='stopStartSlide' title='Select to Pause' class='started skip'>&#9614;&#9614;</a></li>");

            jQuery("ul.rslides_tabs").find('a').not('.skip').each(function() {
                jQuery(this).on('click', function() {
                    jQuery("ul.rslides_tabs").find('a').not('.skip').each(function() {
                        if(jQuery(this).parent().hasClass('rslides_here')) {
                            jQuery(this).find('i').addClass('icon-circle');
                            jQuery(this).find('i').removeClass('icon-circle-empty');
                        } else {
                            jQuery(this).find('i').addClass('icon-circle-empty');
                            jQuery(this).find('i').removeClass('icon-circle');
                        }
                    });
                });
            });

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
                    jQuery("ul.rslides1_tabs").find(":nth-last-child(2)").find("a").click();
                } else {
                    jQuery("li.rslides_here").prev().find("a").not('.skip').click();
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

        jQuery('.form-item .description').each(function(){
            var parent = jQuery(this).parent();
            parent.children('label').after(jQuery(this));
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
            equalheight('.grid-icons .row .wrapper');
            // equalheight('body.front ul.rslides li');
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

(function($, Drupal, window, document, undefined) {
    Drupal.behaviors.crimesWeProsecute = {
        attach: function(context, settings) {
            //handle hoverstate for crimes we prosecute
            $('.grid-icons > .row a > img').hover(
                //handlerIn
                function() {
                    //get .image-hover-hidden src
                    var $elA = $(this);
                    var $elASrc = $elA.attr('src');

                    var $elB = $(this).next('.image-hover-hidden');
                    var $elBSrc = $elB.attr('src');

                    $elA.attr('src', $elBSrc);
                    $elB.attr('src', $elASrc);
                },
                //handlerOut
                function() {
                    //get .image-hover-hidden src
                    var $elA = $(this);
                    var $elASrc = $elA.attr('src');

                    var $elB = $(this).next('.image-hover-hidden');
                    var $elBSrc = $elB.attr('src');

                    $elA.attr('src', $elBSrc);
                    $elB.attr('src', $elASrc);
                }
            );
    	}
    };

    Drupal.behaviors.orderByYears = {
        attach: function(context, settings) {
            //sort year filters in decending order
            var $select = $('#edit-field-year-value, #edit-field-date-value-value-year');
            var $options = $select.find('option');
            $options.sort(function(a, b) {
                if(a.text < b.text) { return 1; }
                else if(a.text > b.text) { return -1; }
                else { return 0; }
            });

            $($select).html($options);
        }
    };

    Drupal.behaviors.addViewAllButton = {
        attach: function(context, settings) {
            //add 'View All' button/link next to 'Apply' button on case reports filter
            var $form = $('#views-exposed-form-case-reports-views-page-1', context);
            var $submit = $form.find('.views-submit-button');
            $submit.append($('<a href="/case-reports/filter?field_category_tid=All&field_report_location_tid=All" class="btn btn-info">View All</a>'));
        }
    };

})(jQuery, Drupal, window, document, undefined);
