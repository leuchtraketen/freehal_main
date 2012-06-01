/*
 * hal2012-tagger2012-builtin.cpp
 *
 *  Created on: 01.06.2012
 *      Author: tobias
 */

#include "hal2012-tagger2012.h"

namespace grammar2012 {

string tagger::__builtin_entity_ends = "i;h;g;f;e;d;"
"c;b;a;er;sie;es;ihm;ihr;ihn;mir;dir;"
"mich;dich;ich;du";

string tagger::__builtin_male_names = "aaron;abraham;achaz;"
"achim;adalbert;adam;adelbert;adelfried;adelhard;adi;"
"adolf;adolph;adrian;aegidius;agilbert;agilmar;alban;"
"albert;albin;albrecht;alex;alexander;alf;alfhard;"
"alfinus;alfons;alfred;alfried;alhard;allmar;alois;"
"aloisius;aloys;aloysius;alphonsus;alrik;altfried;"
"alwin;amadeus;ambros;ambrosius;andreas;andree;andy;"
"annik;anrich;ansbert;anselm;ansgar;anton;antonius;"
"arend;aribert;arist;arkadius;armin;arnbert;arnd;"
"arndt;arne;arnfred;arnfried;arnhart;arnim;arno;"
"arnold;arnolf;arnt;arnulf;aron;arthur;artur;"
"arved;arwed;arwid;arwin;ascan;august;augustin;"
"augustinus;augustus;axel;baldur;balthasar;baptist;"
"barack;barrack;bardo;barnabas;bartho;barthold;"
"bartholomaeus;bartold;bartosch;bastian;beat;benedict;"
"benedikt;benediktus;benjamin;benno;bernd;berndt;"
"bernfried;bernhard;bernhardin;bernhardt;bernhart;berni;"
"berno;bernold;bernulf;bernward;bert;berthold;berti;"
"bertin;bertl;bertold;bertolt;bertram;bertwald;"
"bertwin;birk;bjoern;blasius;bodmar;bodo;bodomar;"
"bolko;bonifaz;boris;boerries;borromaeus;borwin;"
"botho;boto;brandolf;bringfried;bruno;burchard;"
"burckhard;burckhardt;burgfried;burghard;burghardt;"
"burghart;burkard;burkardt;burkhard;burkhardt;burkhart;"
"cai;carl;carlheinz;carsten;caspar;cay;cersten;"
"chris;christfried;christhard;christhardt;christhart;"
"christhelm;christian;christmut;christof;christoph;"
"christopher;christopherus;christophorus;christward;"
"claudius;claus;clemens;conrad;constantin;corbinian;"
"cord;cordian;cordt;cornelius;cornell;cuno;curd;"
"curt;cyrill;dagobert;dagomar;damian;daniel;"
"dankfried;dankhard;dankmar;dankward;dankwart;danny;"
"darius;david;degenhard;degenhardt;degenhart;delf;"
"denis;dennis;denny;desiderius;dethard;detlef;detlev;"
"detmar;detrich;didi;diemo;dierk;dietbald;dietbert;"
"dieter;dieterich;dietfried;dietgar;dietger;diethard;"
"diethardt;diethart;diethelm;diether;diethmar;dietmar;"
"dietmund;dietolf;dietram;dietrich;dietwald;dietwalt;"
"dietwolf;dietwulf;dietz;dimitri;dionys;dirk;ditmar;"
"dittmar;dominic;dominik;dominikus;donat;donatus;"
"eberhard;eberhardt;eberhart;eckard;eckardt;eckart;"
"eckbert;eckehard;eckehardt;eckehart;eckert;eckfried;"
"eckhard;eckhardt;eckhart;eckhold;eddi;edelbert;"
"edelfried;edelhard;edelmut;edelwald;edgar;edmar;"
"edmund;eduard;eduardt;edwin;egbert;egidius;eginald;"
"eginhard;eginhardt;egmont;egomar;egon;ehrenfried;"
"ehrenreich;ehrhard;ehrhardt;ehrmut;eik;eike;einhard;"
"einhardt;eitelbert;eitelfritz;ekhard;ekhardt;ekkard;"
"ekkardt;ekkart;ekkehard;ekkehardt;ekkehart;elbert;"
"elkmar;elmar;emanuel;emeram;emerich;emil;emmeram;"
"emmeran;emmerich;engelbert;engelbrecht;engelhard;"
"engelhardt;engelmar;englbert;enrico;erdfried;erdmann;"
"erdwin;erfried;erhard;erhardt;erhart;eric;erich;"
"erik;erk;erkmar;erlfried;ermenfried;erni;erno;"
"ernst;ernstfried;erwin;eugen;eustachius;ewald;eyck;"
"fabian;falk;falkhard;falkmar;falko;felix;ferdi;"
"ferdinand;ferdinandus;ferfried;ferhard;ferhart;"
"fidelius;firma;florentin;florentius;florenz;florian;"
"folker;folkhard;folkhardt;folkhart;folko;follrich;"
"frank;frankmut;franz;franziskus;franzl;franzpeter;"
"fred;freddy;freimund;freimut;freimuth;freiwald;"
"fridolin;fridulf;fried;friedbald;friedberg;friedbert;"
"friedebert;friedeborn;friedel;friedemann;friedemar;"
"frieder;friedewald;friedhard;friedhardt;friedhart;"
"friedhelm;friedhold;friedjoerg;friedl;friedlieb;"
"friedmar;friedmut;friedmuth;friedolf;friedolin;"
"friedrich;friedwald;friedwin;frithjof;fritz;frohmund;"
"frohmut;frohwald;frowin;gabriel;gangolf;gebhard;"
"gebhart;gelbert;georg;gerald;gerd;gerdfried;gerdt;"
"gereon;gerfried;gerhard;gerhardt;gerhart;gerlach;"
"german;germar;germo;germut;gernfried;gernod;gernot;"
"gernulf;gero;gerold;gerolf;gerrit;gert;gerthold;"
"gertmund;gertwin;gerwald;gerwin;giesbert;gisbert;"
"giselbert;giselher;gismar;godo;golo;gordian;goswin;"
"gottfried;gotthard;gotthardt;gotthart;gotthelf;"
"gotthilf;gotthold;gottlieb;gottlob;gottreich;gottwald;"
"gottwaldt;gottwalt;goetz;gregor;grimbert;guido;"
"gundhard;gundhart;gundmar;gundolf;gunfried;gunhard;"
"gunnar;gunter;guenter;gunthard;gunther;guenther;"
"guntmar;guntram;gustav;gustel;gustl;hadubert;hagen;"
"haimo;hanfried;hannes;hanno;hanns;hannsgeorg;"
"hannsjoerg;hannsjuergen;hannspeter;hans;hansdieter;"
"hansfried;hansfriedrich;hansgeorg;hansgerd;hansguenter;"
"hansguenther;hansheinz;hanshelmut;hanshermann;hansi;"
"hansjakob;hansjoachim;hansjochen;hansjoerg;hansjoern;"
"hansjuergen;hanskarl;hansl;hansludwig;hansmartin;"
"hansotto;hanspeter;hanswalter;hanswerner;hanswilhelm;"
"hanswolf;harald;hardi;hardo;hardy;hariolf;harri;"
"harribert;harro;harry;hartfried;hartger;hartlieb;"
"hartmann;hartmund;hartmut;hartmuth;hartwald;hartwig;"
"hartwin;hasko;hasso;hatto;haug;haymo;heidebert;"
"heiderich;heidfried;heiko;heilmuth;heilwig;heimar;"
"heimbert;heimfrid;heimhart;heimo;heimwald;heiner;"
"heinfried;heini;heino;heinolf;heinrich;heintje;"
"heinz;heinzdieter;heinzfried;heinzhermann;heinzjoerg;"
"heinzjuergen;heinzkarl;heinzpeter;heinzwerner;helbert;"
"helfred;helfrid;helfried;helge;helgert;helgomar;"
"hellfried;hellmut;hellmuth;helm;helmar;helmbrecht;"
"helmfried;helmo;helmrich;helmut;helmuth;helmward;"
"helwart;helwig;helwin;hendrik;henning;henrik;henry;"
"herald;herbert;herfried;herger;heribert;herko;"
"hermann;hermfried;herold;herrmann;herwald;herward;"
"herwart;herwarth;herwig;heyko;heyo;hieronimus;"
"hieronymus;hilar;hilarius;hildebert;hildebrand;"
"hildemar;hildmar;hilfried;hilmar;hippolytus;hoimar;"
"holger;holk;holm;holmer;horst;horstfried;horstmar;"
"hubert;hubertus;hugo;humbert;ignatz;ignaz;ilrich;"
"immanuel;ingbert;ingfried;inghard;ingmar;ingo;"
"ingobert;ingolf;ingomar;ingowart;ingram;inkmar;"
"innozenz;irenaeus;ireneus;irmfried;irmin;irminfried;"
"irmo;isbert;isfried;isidor;iven;ivo;jacob;jakob;"
"jan;janfried;jannick;jean;jens;jo;joachim;jobst;"
"jochen;joel;johann;johannes;jonas;jonathan;jonny;"
"jons;joerg;joergfried;joern;joscha;joschka;josef;"
"joseph;jost;josua;julian;julius;jupp;juergen;"
"just;justus;kai;kajetan;karl;karlernst;karleugen;"
"karlfried;karlfrieder;karlfriedrich;karlgeorg;karlguenter;"
"karlhans;karlheiner;karlheinrich;karlheinz;karlhorst;"
"karli;karlmann;karlotto;karlpeter;karsten;kasimir;"
"kaspar;kay;kersten;kevin;kilian;klaudius;klaus;"
"klausdieter;klausjuergen;klauspeter;klemens;knut;knuth;"
"konrad;konradin;konstantin;korbinian;kord;kornelius;"
"kristian;kruno;kunibert;kuno;kurd;kurt;kurth;"
"lambert;landolf;lars;laurentius;laurenz;leander;"
"leberecht;lebrecht;lenard;lenz;leo;leon;leonhard;"
"leonhardt;leopold;leuthold;lewin;liborius;liebhard;"
"liebhardt;lienard;lienhard;lienhardt;lindmar;linhart;"
"lois;loisl;lorenz;lothar;louis;lucas;ludger;"
"ludolf;ludwig;luitpold;luitwin;lukas;lutz;maic;"
"maik;malte;manfred;manfried;manhard;manhardt;"
"manhold;manuel;marbod;marc;marcel;marcellus;marco;"
"marcus;marfried;margolf;marian;mario;marius;mark;"
"marko;markolf;markus;markward;markwart;marold;"
"martin;marzel;marzellus;mathias;mathis;matthaeus;"
"matthias;maurus;max;maximilian;maximillian;maxl;"
"mayk;medard;medardus;meik;meikel;meinard;meinfried;"
"meinhard;meinhart;meinold;meinolf;meinrad;meinulf;"
"melchior;merten;micha;michael;michel;michl;mike;"
"mirco;mirko;mischel;moritz;nathanael;neidhard;"
"neidhardt;neithard;neithart;nepomuk;nick;nicki;"
"nicky;nico;nicolas;nicolaus;niels;niklas;niklaus;"
"niko;nikodem;nikodemus;nikolas;nikolaus;nils;"
"norbert;nordfried;norfried;norman;norwin;notfried;"
"notker;nunzius;odilo;olaf;olf;oliver;olli;ornulf;"
"ortfried;ortmund;ortwin;oskar;osmund;oswald;oswin;"
"otbert;otfried;otger;otheinrich;othmar;otmar;"
"ottfried;otthard;otthart;otthein;ottheinrich;ottheinz;"
"otthold;ottkar;ottmar;otto;ottobert;ottokar;ottomar;"
"ottwin;otwald;otwin;pankratius;pankraz;pascal;"
"patrick;paul;paulfried;paulinus;paulus;peter;philip;"
"philipp;phillip;phillipp;phillippus;pierre;pirmin;"
"pius;quintus;quirin;raik;raimar;raimer;raimond;"
"raimund;rainald;rainer;rainhard;rainhardt;rainmund;"
"ralf;ralph;randolf;randolph;raphael;rasso;raymund;"
"reginbert;reginhardt;reimo;reimund;reinald;reinar;"
"reinbert;reiner;reinfried;reinhard;reinhardt;reinhart;"
"reinhold;reinholdt;reinmar;reino;reinold;reinulf;"
"reinwald;rembert;remigius;renald;renatus;renold;"
"rezzo;richard;rico;rigbert;rigo;rigobert;rimbert;"
"ringo;robby;robert;roberto;rochus;roderich;roger;"
"roland;rolf;roman;romanus;ronald;ronny;roselius;"
"rotger;rothmund;rouven;ruben;rudenz;rudger;rudi;"
"ruediger;rudolf;rudolph;runald;runfried;rupert;"
"rupertus;ruppert;rupprecht;ruprecht;rutger;ruetger;"
"ruthard;samuel;sascha;schorsch;sebald;sebastian;"
"seibold;sepp;seppl;sergius;severin;sibrand;sidonius;"
"siegbert;siegenot;siegert;siegfrid;siegfried;sieghard;"
"sieghardt;sieghart;sieghelm;siegmar;siegmund;siegulf;"
"siegwald;siegward;siegwart;sigbert;sigenot;sigfrid;"
"sigfried;siggi;sighard;sighart;sighelm;sigi;"
"sigisbert;sigishelm;sigismund;sigmar;sigmund;sigmut;"
"sigolf;sigurd;sigward;sigwart;silvester;silvio;"
"simon;simpert;sixtus;soenke;sonnfried;sonnhard;"
"sonnhardt;soeren;stanislaus;stefan;steff;steffan;"
"steffen;steph;stephan;stephanus;steve;steven;"
"sturmhard;sturmius;sven;swen;sylvester;sylvio;"
"sylvius;tankred;tassilo;tell;thaddaeus;thankmar;"
"thassilo;theo;theobald;theobert;theodor;theofried;"
"theohold;theophil;theophilus;thielo;thiemo;thilo;"
"thomas;thoralf;thorsten;tiberius;tiburtius;tiemo;"
"til;till;tillmann;tilman;tilmann;tilo;tim;timmo;"
"timo;timon;timotheus;tino;titus;tobias;tom;tommy;"
"toni;toens;tony;toralf;torben;torge;torsten;"
"traugott;trauhard;treufried;treuhard;treuhart;tristan;"
"trudbert;trutz;udo;ulf;ulfried;uli;ulli;ullrich;"
"ulrich;ulvi;ulwin;uranius;urban;ursus;utho;uto;"
"utto;utz;uve;uwe;valentin;valerian;veit;veith;"
"velten;viktor;vincenz;vinzent;vinzenz;vitalis;vitus;"
"volkard;volkart;volkbert;volker;volkhard;volkhardt;"
"volkhart;volkher;volkmar;volkrad;volkward;volkwart;"
"vollrath;volrad;walbert;waldemar;waldfried;walfried;"
"walter;walther;walti;warnfried;wastl;wedig;wedigo;"
"weikhard;welf;welfhard;wendel;wendelin;wendelinus;"
"wendemar;wennemar;wenzel;werner;wernfried;wernhard;"
"wernher;wichard;wido;wiegand;wiegbert;wieghart;"
"wieland;wiethold;wigand;wigbert;wiglaf;wilbert;"
"wilbrand;wilbrandt;wilderich;wilfrid;wilfried;wilhard;"
"wilhardt;wilhelm;willehad;willhart;willi;willibald;"
"willibert;willmar;willrich;willy;wilmar;wilmhard;"
"wilmut;winand;winfrid;winfried;wingolf;winnimar;"
"winrich;withold;witmar;wittmar;wladislaus;woldemar;"
"wolf;wolfbert;wolfdieter;wolfdietrich;wolfeckart;wolff;"
"wolfgang;wolfguenter;wolfhard;wolfhardt;wolfhart;"
"wolfhelm;wolfhermann;wolfmar;wolfram;wolfrid;wolfried;"
"wolrad;wulf;wulfhard;wulfhardt;wunibald;wunnibald;"
"xaver;xaverius;york;zacharias;zeno";
string tagger::__builtin_female_names = "";
vector<string> tagger::builtin_entity_ends;
vector<string> tagger::builtin_male_names;
vector<string> tagger::builtin_female_names;
vector<string> tagger::custom_names;

}
