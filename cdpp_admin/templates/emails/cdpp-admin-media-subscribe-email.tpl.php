<?php

/**
 * @file
 * Default theme implementation to display media notification email.
 *
 * Variables:
 * - $logo_url: Site logo URL fully processed and absolute.
 * - $title: (string) The email title.
 * - $intro_text: (string) The processed Intro text,
 *   run through check_markup.
 * - $closing_text: (string) The processed closing text,
 *   run through check_markup.
 * - $unsub_subject: (string) the desired unsubscribe subject.
 * - $unsub_subject_encoded: (string) the desired unsubscribe subject
 *   url encoded.
 *   - (string) The filter id to use.
 * - $list: (array)
 *   - (array) [list of news items - usually only 1]
 *     - 'title': (string)The news unescaped title.
 *     - 'date': (string)A date in the format: 'DD/YY/YYYY'.
 *     - 'time': (string)The time of the node publish date e.g. '3:00PM'.
 *     - 'timezone': (string)The shorthand timezone of the webinar: e.g. 'AEST'.
 *     - 'link': (string) The link address (raw e.g. "node/1234")
 *       to view the news article.
 * - $unsub_email_address:
 *
 * @see hook_preprocess_cdpp_admin_media_subscribe_email()
 *
 * @ingroup themeable
 */
?>

<?php /*
* The below is the output from the email prototype in production mode
* To generate the markup:
* - go to the /frontend-utilities/ folder
* - npm i
* - gulp --production
* - go to the /frontend-utilities/email-templates/tmp/index.build.html file
* - copy past all into the section below
*/?>

<!DOCTYPE html>
<html lang="en" style="font-family: Helvetica,Arial,sans-serif; font-size: 62.5%; word-break: break-word;">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- Use [dash dash production] flag to get inline css-->

    <title><?php print $title; ?></title>
    <style>
      .pageContainer p, .pageContainer li { color: #333; }

    </style>
  </head>
  <body style="background: #f9f9f7; color: #333; font-size: 16px; line-height: 1.6; margin: 0;">
    <!-- preview text in outlook -->
    <div style="display:none;font-size:1px;color:#333333;line-height:1px;max-height:0px;max-width:0px;opacity:0;overflow:hidden;">
      <?php print $title; ?>
    </div>
        <div class="safetyPadding" style="padding-left: 20px; padding-right: 20px;">
              <div class="pageContainer" style="margin-left: auto; margin-right: auto; max-width: 700px; width: 100%;">
                    <div class="logo" style="margin-bottom: 20px; padding-top: 20px;"><img class="logo__img" src="<?php print $logo_url; ?>" alt="CDPP: Australia's Federal Prosecution Service logo">
                    </div>
                    <div class="contentContainer" style="background: #fff; border: 2px solid #876e4c; padding: 20px;">
                      <h1 style="color: #876e4c; font-weight: 400; margin-bottom: 5px; margin-top: 0; font-size: 28px;">Commonwealth Director of Public Prosecutions</h1><?php print $intro_text; ?>
                      <?php foreach ($list as $article): ?>
                          <p class="highlightedLink" style="background: #f9f9f7; border: 1px solid #876e4c; color: #333; display: block; font-size: 1.2em; margin: 20px 0; padding: 10px 20px;"><a class="highlightedLink__link" href="<?php print $article['link']; ?>" style="color: #444; display: block; text-decoration: none;"><?php print $article['title']; ?></a>
                          </p>
                      <p style="color: #333; margin: 20px 0;">Publication date: <?php print $article['date']; ?></p>
                      <?php endforeach; ?>
                      <?php print $closing_text; ?>
                    </div>
                    <div class="footer" style="margin-top: 20px; padding-bottom: 20px;">
                      <p style="color: #333; font-size: .8em; margin: 20px 0; margin-bottom: 0; margin-top: 0; text-align: center;">To unsubscribe, please email <a href="mailto:<?php print $unsub_email_address; ?>?Subject=<?php print $unsub_subject_encoded; ?>" style="color: #444; text-decoration: underline;"><?php print $unsub_email_address; ?></a> with the subject line <br> "<?php print $unsub_subject; ?>".
                      </p>
                    </div>
              </div>
        </div>
  </body>
</html>
