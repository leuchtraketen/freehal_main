#!/usr/bin/php-cgi

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
    <title>FreeHAL - free software intelligence</title>
    <style type="text/css">
	* {
		font-size: 10pt;
	}
	h1, h2, h3, h4, h5, h6 {
		padding-bottom: 5px;
		border-bottom: 1px solid silver;
	}
	h2 {
		font-size: 16pt !important;
	}
	h3, .comment .title,  .comment .title a {
		font-size: 13pt !important;
		margin-bottom: 0;
		font-weight: normal;
		color: black;
	}
	h4 {
		font-size: 12pt !important;
		margin-bottom: 0;
		font-weight: normal;
	}
	li {
		margin-left: -30px;
		list-style-type: none;
	}
	.links li { display: inline; margin: 0; margin-right: 10px; } 
	img { border: none; }
	a { text-decoration: none; }
	pre {
		border: none;
		margin-left: 13px;
		padding: 5px;
		border-left: 6px solid grey;
		border-top: 2px solid grey;
		border-bottom: 1px solid grey;
		background: white;
	}
	.comment .content, .node .content, .pre {
		border: none;
		margin-left: 0px;
		background: white;
		padding-top: 5px;
		margin-top: 10px;
	}
	.box .title, #comment-controls { display: none; }
	#user-login-form .form-item {
		border: 1px solid #eee;
                padding: 5px;
		width: 150px !important;
	}
	#comment-form .form-item {
		border: 1px solid #eee;
                padding: 5px;
		width: 99% !important;
	}
	#edit-format-1-wrapper { display: none; }
	#edit-format-2-wrapper { display: none; }
	#edit-format-3-wrapper { display: none; }
	#edit-format-4-wrapper { display: none; }
	#edit-format-5-wrapper { display: none; }
	.item-list li { display: inline; margin-left: 0px; }
td, th {
	text-align: left;
	vertical-align: top;
	padding: 5px;
}
th {
	background: #20386e;
	color: white;
}
.container, .container a, .date {
	background: #5480d0;
	color: white;
	font-weight: normal;
}
td.single {
	border: none;
	border-left: 5px solid silver;
	border-top: 2px solid silver;
}
table {
	border-collapse: collapse;
	width: 95%;
/*	background: #eee;*/
}
a, a:visited {
        color: #021a3a;
}
a:active {
        color: #bbd2f1;
}
a:hover {
        color: #3f7dd2;
}

iframe {
	width: 80%;
	height: 500px;
}

    </style>
</head>
<body style="background: url(http://freehal.org/start-bg.png) #042042 no-repeat;">

<a href="/" style="position: absolute; top: 20px; left: 2%; z-index: 10;">
	<img src="http://freehal.org/start-logo-wide.png" />
</a>
<!--<a href="/start-screenshot-big.png"  style="position: absolute; top: 10px; right: 10px; z-index: 10;"><img src="/start-screenshot.png" style="border: none;" /></a>-->

<div style="position: absolute; top: 30px; right: 310px; padding: 10px; font-family: Sans, Verdana, Tahoma; height: 130px !important;">
	<a href="start-buttom-down.png"><img src="http://freehal.org/start-button-down.png" style="border: none;" /></a>
</div>
<div style="background: white repeat-x url(http://freehal.org/start-innerbg.gif); border: 1px solid #4c77a3; position: absolute; top: 10px; left: 1%; right: 1%; padding: 20px;
	padding-left: 210px; padding-top: 160px; font-size: 10pt; font-family: Sans, Verdana, Tahoma; min-height: 570px;">

	<p style="margin-right: 280px;">
	</p>
	<p style="margin-right: 80px;">
	</p>
	<p style="margin-right: 0px;">

	</p>

</p>
<center style="color: #1c3368;"><b>Currently the only language understood by FreeHAL's online demo is German, we are working on an english version.</b></center>
<ul>
<li>- Freehal ist ein Forschungsprojekt und kein Chatbot. Freehal arbeitet nach
den Grundlagen der deutschen Sprache. </li>

<li>- Wenn ihr euch mit Freehal unterhaltet, dann bitte in klaren, kurzen S&auml;tzen.
Freehal zerlegt die S&auml;tze nach den Regeln der deutschen Grammatik &nbsp;&nbsp;und sucht
auf dieser Basis nach m&ouml;glichen Antworten. </li>

<li>- Auf Eingaben wie "Und was machste", "Was haste an", "Wie alt" oder "Woher"
werdet ihr keine vern&uuml;nftige Antwort erhalten. </li>

<li>- Mit vern&uuml;nftigen Dialogen helft ihr auch dem Team Freehal weiter zu
entwickeln und so zur positiven Entwicklung des Projektes beizutragen. </li>

<li><br /><div
 style="text-align: center;"><span
 style="font-weight: bold;">Eine Onlineversion von Freehal auf einem Linuxserver
.</span></div></li>

</ul><br />


<form method="post">
<input type="text" name="q" />
<button type="submit">Ask</button>
</form>
<?php

ob_implicit_flush(true);
ob_end_flush();

#chdir("/var/www/web0/html/hal2009");
$_IP = $_SERVER[HTTP_X_FORWARDED_FOR];
if (!$_IP) $_IP = $_SERVER['X-Forwarded-For'];
if (!$_IP) $_IP = $_SERVER[X_HTTP_FORWARD_FOR];
if (!$_IP) { $_IP = $_SERVER[REMOTE_ADDR]; }
if ($_POST[q]) {

	$_POST[q] = str_replace("ä", "ae", $_POST[q]);
	$_POST[q] = str_replace("ö", "oe", $_POST[q]);
	$_POST[q] = str_replace("ü", "ue", $_POST[q]);
	$_POST[q] = str_replace("ß", "ss", $_POST[q]);
	$_POST[q] = str_replace("Ä", "Ae", $_POST[q]);
	$_POST[q] = str_replace("Ö", "Oe", $_POST[q]);
	$_POST[q] = str_replace("Ü", "Ue", $_POST[q]);

	$time = time();

$q = str_replace("\"", "", $_POST[q]);

$cgi_r = fopen("_cgi_request", "w"); // for hal
fwrite($cgi_r, $q);
fwrite($cgi_r, "\n");
fclose($cgi_r);
$cgi_r = fopen("log/i-".$_IP, "w"); // for php
fwrite($cgi_r, $q);
fwrite($cgi_r, "\n");
fclose($cgi_r);
$cgi_u = fopen("_cgi_user", "w");
fwrite($cgi_u, $_IP);
fwrite($cgi_u, "\n");
fclose($cgi_u);
@system("chmod 0777 _cgi_request");
@system("chmod 0777 _cgi_user");
unlink("_done");

$cgi_u = fopen("log/h-o", "w");
fwrite($cgi_u, file_get_contents("log/o-".$_IP));
fclose($cgi_u);
}

$h_1 = file_get_contents("log/o-".$_IP);
$h_2 = file_get_contents("log/h-o");
if (!$h_2 && $h_1) {
	echo "<div class='pre'>";
	echo "<iframe style='border: none;' src='iframe-dialog.php?t=60&ip=".$_IP."></iframe>";
	echo "</div>";
}
else if ($h_1 != $h_2) {
	$cgi_u = fopen("log/h-o", "w");
	fclose($cgi_u);

	$time = time() - $time;
@mkdir("log");
	$content_of_log = @file_get_contents("log/$_IP");
	$logf = fopen("log/$_IP", "w");
	fwrite($logf, "<span style=\"color: silver !important;\">$_IP:</span> <b>".file_get_contents("log/i-".$_IP)."</b>\n");
	fwrite($logf, "<span style=\"color: silver !important; width: 150px !important; display: inline-block;\">FreeHAL:</span>");
	$datei = file("log/o-".$_IP);
	$letzte_zeile = array_pop($datei);
	fwrite($logf, trim($letzte_zeile));
	fwrite($logf, " ($needed_time sec)\n");  
	fwrite($logf, $content_of_log);
	fclose($logf);

	echo "<div class='pre'>";
	echo "<iframe style='border: none;' src='iframe-dialog.php?t=60&ip=".$_IP."></iframe>";
	echo "</div>";
}
else {
	echo "<div class='pre'>";
	echo "<iframe style='border: none;' src='iframe-dialog.php?t=3&ip=".$_IP."></iframe>";
	echo "</div>";
}
?>

</div>

<div style="position: absolute; top: 172px; left: 1%; padding: 20px; font-family: Sans, Verdana, Tahoma; background: url(http://freehal.org/start-leftbar.png) no-repeat; min-height: 500px; width: 200px; z-index: 90;">

<b>FreeHAL Project</b>
<ul>
  <li><a href="http://doc.freehal.org/">Overview <img src="http://freehal.org/modules/languageicons/flags/en.png" style="width: 18px; height: 9px; margin-left: 2px;" /></a></li>
  <li><a href="http://doc.freehal.org/en/Documentation">Documentation <img src="http://freehal.org/modules/languageicons/flags/en.png" style="width: 18px; height: 9px; margin-left: 2px;" /></a></li>
  <li><a href="http://www.freehal.org/forum">Forum <img src="http://freehal.org/modules/languageicons/flags/en.png" style="width: 18px; height: 9px; margin-left: 2px;" /></a></li>
  <li><a href="http://download.freehal.org/">Download <img src="http://freehal.org/modules/languageicons/flags/en.png" style="width: 18px; height: 9px; margin-left: 2px;" /></a></li>
  <li><a href="http://doc.freehal.org/de/Entwicklertagebuch">News <img src="http://freehal.org/modules/languageicons/flags/de.png" style="width: 18px; height: 9px; margin-left: 2px;" /></a></li>
  <li><a href="http://boinc.freehal.org/">BOINC <img src="http://freehal.org/modules/languageicons/flags/en.png" style="width: 18px; height: 9px; margin-left: 2px;" /></a></li>
</ul>

</ul>
</div>



</body>
</html>
