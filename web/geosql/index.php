<?php 
ini_set('display_errors', 1);
error_reporting(E_ALL);

include "db.php";

const SECURITY = '[[55.997324986637025,92.78563499450684],[55.99118059635058,92.7853775024414],[55.99146863646352,92.80056953430176],[55.99866894182232,92.79979705810547],[55.997324986637025,92.78563499450684]]';
const SEC_POINT = '55.9945109 92.7975869';

if(isset($_POST['add'])){
	if(!(isset($_POST['latLng']) && isset($_POST['text']))){
		echo 'Не все обязательные поля включены';
		exit;
	}
	
	$test = true;
	$test &= strripos($_POST['latLng'], explode(' ', SEC_POINT)[0]) === false;
	$test &= strripos($_POST['latLng'], explode(' ', SEC_POINT)[1]) === false;
	$test &= strripos($_POST['text'], explode(' ', SEC_POINT)[0]) === false;
	$test &= strripos($_POST['text'], explode(' ', SEC_POINT)[1]) === false;
	if(!$test){
		echo 'Будьте тише с такими координатами, не передавайте их по незащищенному каналу. Ваш руководитель.';
		exit;
	}
	
	$latLng = $_POST['latLng'];
	$text = $_POST['text'];
	
	$sec_poly = json_decode(SECURITY);
	$coords = [];

	foreach ($sec_poly as $c) {
		$coords[] = $c[0] . ' ' . $c[1];
	}
	$coords = 'POLYGON((' . implode(',', $coords) . '))';
	$test_point = "POINT($latLng)";
	
	
	$stmt = $pdo->prepare("SELECT ST_Contains(PolygonFromText(:coords),GeomFromText(:test_point)) as test;"
	);
	$stmt->execute([':coords'=>$coords, ':test_point'=>$test_point]);
	if($stmt->fetch()['test']){
		echo 'Эта область защищена от записи';
		exit;
	}
	
	$sql = "INSERT INTO `notes`(`position`, `text`, `secret`) VALUES  (GeomFromText('POINT($latLng)'),'$text', 'none');";
	try{
		$res = $pdo->query($sql);
	} catch(Exception $e) {
		$error = "sql:".$sql." error:".$e;
	}
}

if(isset($_POST['polygon'])){
	$data = [];
	$polygon = json_decode($_POST['polygon']);
	$polygon[] = $polygon[0];
	
	$coords = [];

	foreach ($polygon as $c) {
		$coords[] = $c[0] . ' ' . $c[1];
	}
	$coords = 'POLYGON((' . implode(',', $coords) . '))';
	
	$stmt = $pdo->prepare("SELECT id, text, asText(position) as point FROM notes WHERE ST_Contains(PolygonFromText(:coords), notes.position) LIMIT 10;");
	try{
		$stmt->bindValue(':coords', $coords , PDO::PARAM_STR);
		$stmt->execute();

		$models = $stmt->fetchAll();
		
		$jsonModel = [];
		for($i = 0; $i < count($models); $i++){
			list($lat,$lng) = explode(" ", substr(substr($models[$i]['point'], 6), 0, -1));
			
			$content = '<p>';
            $content .= $models[$i]['text'];
            $content .= '</p>';

			$jsonModel[] =  json_encode(array(
                floatval($lat),
				floatval($lng),
                $content
            ));
		}
	} catch(Exception $e) {
		$error = "sql:".$sql." error:".$e;
	}
}

?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

	<meta name="language" content="ru" />
    <script src="./jquery-2.1.4.min.js"></script>
    <script src="https://maps.googleapis.com/maps/api/js"></script>
    <script src="./markerclusterer.js"></script>
    <script src="./polygon.min.js"></script>
	
	<style>
    .gmap-control-container {
        margin: 5px;
    }
    .gmap-control {
        cursor: pointer;
        background-color: -moz-linear-gradient(center top , #FEFEFE, #F3F3F3);
        background-color: #FEFEFE;
        border: 1px solid #A9BBDF;
        border-radius: 2px;
        padding: 0 6px;
        line-height: 160%;
        font-size: 12px;
        font-family: Arial,sans-serif;
        box-shadow: 2px 2px 3px rgba(0, 0, 0, 0.35);
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -o-user-select: none;
        user-select: none;
    }
    .gmap-control:hover {
        border: 1px solid #678AC7;
    }
    .gmap-control-active {
        background-color: -moz-linear-gradient(center top , #6D8ACC, #7B98D9);
        background-color: #6D8ACC;
        color: #fff;
        font-weight: bold;
        border: 1px solid #678AC7;
    }
    .gmap-control-legend {
        position: absolute;
        text-align: left;
        z-index: -1;
        top: 20px;
        right: 0;
        width: 150px;
        height: 80px;
        font-size: 10px;
        background: #FEFEFE;
        border: 1px solid #A9BBDF;
        padding: 10px;
        box-shadow: 2px 2px 3px rgba(0, 0, 0, 0.35);
    }
    .gmap-control-legend ul {
        margin: 0;
        padding: 0;
        list-style-type: none;
    }
    .gmap-control-legend li {
        line-height: 160%;
    }
</style>
</head>


<body>

<div class="row">
    <div class="col-md-12">
        
            <div class="row">
                <div class="col-md-2">
					<button id="search" onclick="return search();">Поиск</button>
					<button id="reset" onclick="return reset_polygon();">Стереть полигон</button>
                </div>
                <div class="col-md-offset-2 col-md-4">
                    Укажите область на карте, отмечая вершины многоугольника
                </div>
            </div>
        <form id="search-form" method="POST" action="./index.php">
			<?php if(!isset($_POST['polygon'])): ?>
			<input type="hidden" name="polygon" id="input-polygon">
			<?php else: ?>
			<input type="hidden" name="polygon" id="input-polygon" value="<?=$_POST['polygon'];?>">
			<?php endif; ?>
		</form>
		<form id="add-form" action="./index.php" method="POST" style="display:none;">
			<input id="input-latLng" type="hidden" name="latLng">
			<input id="input-text" type="text" name="text" placeholder="Введите текст заметки">
			<input type="submit" name="add" value="Отправить" onclick="return check();">
		</form>
        <div id="googleMap" style="height:600px;width:100%"></div>
    </div>
</div>

<script>
	<?php if(isset($_GET['debug'])): ?>
	var error = "<?= isset($error) ? $error : ''; ?>";
	<?php endif; ?>
	
	var latLng = false;

    var mapProp = {
        center:new google.maps.LatLng(56,92.8),
        zoom:8,
        mapTypeId:google.maps.MapTypeId.ROADMAP
    };

    var map = new google.maps.Map(document.getElementById("googleMap"),mapProp);
	google.maps.event.addListener(map, "rightclick", function(event) { 
		latLng = event.latLng.lat() + " " + event.latLng.lng(); 
		$('#add-form').show(400);
	});
	
	var sl = <?=SECURITY?>;
	var securityLocation = [];
	
	for (var index = 0; index < sl.length; ++index) {
		var dot = new google.maps.LatLng(sl[index][0], sl[index][1]);
		securityLocation.push(dot);
	}
			
	var sec_polygon = new google.maps.Polygon({
        paths: securityLocation,
        strokeColor: "#FF0000",
        strokeOpacity: 0.8,
        strokeWeight: 2,
        fillColor: "#FF0000",
        fillOpacity: 0.5,
        map: map
    });

    var creator, polygon;
    var locations = [
        <?php if(isset($jsonModel))
			echo implode(',', $jsonModel);
        ?>
    ];

    

    var markerClusterer = init_markers(map);

    init_creator(map);
	
	//reset
    function reset_polygon(){
		if(polygon)
			polygon.setMap(null);
		
		creator.destroy();
		creator=null;
		creator=new PolygonCreator(map);
		return false;
	};
	
	function search(){
		if(!creator){
			alert('Что то пошло не так...');
			return false;
		}
		data = creator.pen.getDataJson();
		if(!data){
			alert('Укажите сначала полигон');
			return false;
		}
		$('#input-polygon').val(JSON.stringify(data));
		$('#search-form').submit();
	};
	
	function check(){
		if(!latLng)
			return false;
		if(!$('#input-text').val()){
			alert('Заполните текст');
			return false;
		}
		$('#input-latLng').val(latLng);
		//$('#add-form').submit();
	}

    function init_creator(map) {

        creator = new PolygonCreator(map);

        var coords = [];
		var data_polygon = $('#input-polygon').val();
        if(data_polygon) {
            var obj = JSON.parse(data_polygon);
            for (var index = 0; index < obj.length; ++index) {
                var dot = new Dot(new google.maps.LatLng(obj[index][0], obj[index][1]), map, creator.pen);
                coords.push(dot);
            }
			coords.push(new Dot(new google.maps.LatLng(obj[0][0], obj[0][1]), map, creator.pen)); // full
			creator.loadPolygon(coords);
            
        }

    }

    function init_markers(map){

        var markers = [];
        var infowindow = new google.maps.InfoWindow({
        });
        for (var i = 0; i < window['locations'].length; i++) {
            var location = window['locations'][i];
            var myLatLng = new google.maps.LatLng(location[0], location[1]);
            var marker = new google.maps.Marker({
                position: myLatLng,
                //title: location[2]
                html: location[2]
            });

            google.maps.event.addListener(marker, 'click', function() {
                infowindow.setContent(this.html);
                infowindow.open(map, this);
            });
            markers.push(marker);
        }

        return new MarkerClusterer(map, markers, {
            maxZoom: 13,
            gridSize: 50,
            styles: null
        });
    }
	
	//?php if($_GET['debug']): ?>
	//console.log(error)

</script>