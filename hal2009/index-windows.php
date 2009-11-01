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
		background: white url(http://freehal.org/start-commentbg.png) no-repeat;
		padding: 20px;
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

<i>This FreeHAL only speaks German! An english version is not yet available.</i><br />


<form method="post">
<input type="text" name="q" />
<button type="submit">Ask</button>
</form>
<?

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
?> <pre style="display: none; height: 300px; overflow: auto; float: right; max-width: 800px;" id="sc"><?
$q = str_replace("\"", "", $_POST[q]);
$handle = popen('hal2009-online-demo.exe "'.$q.'"', "r");
$_log = fopen("last_question.log", "w");
while ($read = fread($handle, 4000)) {
#	echo $read;
	fwrite($_log, $read);
}
pclose($handle);
fclose($_log);
?> </pre>
<script type="text/javascript">
document.getElementById("sc").style.height = 500;
document.getElementById("sc").style.width = 200;
document.getElementById("sc").style.float = "right";
</script>

<?
	$time = time() - $time;
mkdir("log");
	$content_of_log = file_get_contents("log/$_IP");
	$logf = fopen("log/$_IP", "w");
	fwrite($logf, "$_IP: $_POST[q]\n");
	fwrite($logf, "<span style=\"width: 150px !important; display: inline-block;\">FreeHAL:</span>");
	$datei = file("lang_de/output.history");
	$letzte_zeile = array_pop($datei);
	fwrite($logf, trim($letzte_zeile));
	fwrite($logf, " ($time sec)\n");
	fwrite($logf, $content_of_log);
	fclose($logf);
}
echo "<div class='pre'>";
echo str_replace("\n", "<br>", file_get_contents("log/$_IP"));
echo "</div>";
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
