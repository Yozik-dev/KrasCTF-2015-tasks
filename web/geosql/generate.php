<?php
include "db.php";
include "crc.php";

const SECURITY = '[[55.997324986637025,92.78563499450684],[55.99118059635058,92.7853775024414],[55.99146863646352,92.80056953430176],[55.99866894182232,92.79979705810547],[55.997324986637025,92.78563499450684]]';

const ID_SECRET = 2893;
const SEC_POINT = '55.9945109 92.7975869';
const SEC_SECRET = 'flag is:#880055535350';

const TOTAL = 10000;


function float_rand($Min, $Max, $round=0){
    //validate input
    if ($Min>$Max) { $min=$Max; $max=$Min; }
        else { $min=$Min; $max=$Max; }
    $randomfloat = $min + mt_rand() / mt_getrandmax() * ($max - $min);
    if($round>0)
        $randomfloat = round($randomfloat,$round);

    return $randomfloat;
}


$res = $pdo->query('TRUNCATE notes');


$sec_poly = json_decode(SECURITY);
$sec_coords = [];

foreach ($sec_poly as $c) {
	$sec_coords[] = $c[0] . ' ' . $c[1];
}
$sec_coords = 'POLYGON((' . implode(',', $sec_coords) . '))';


for($i=0; $i<TOTAL;){

	if($i == ID_SECRET){
		$latLng = SEC_POINT;
		$secret = SEC_SECRET;
	} else {
	
		$lat = float_rand(0, 150, 7) - 75;
		$lng = float_rand(0, 360, 7) - 180;
		if($i % 10 == 0){
			switch (rand(1,2)){
				case 1: 
					$lat = floatval(explode(' ', SEC_POINT)[0]);
				case 2: 
					$lng = floatval(explode(' ', SEC_POINT)[1]);
			}
		}
		$latLng = "$lat $lng";
		
		$test_point = "POINT($latLng)";
		$stmt = $pdo->prepare("SELECT ST_Contains(PolygonFromText(:coords),GeomFromText(:test_point)) as test;"
		);
		$stmt->execute([':coords'=>$sec_coords, ':test_point'=>$test_point]);
		if($stmt->fetch()['test']){
			continue;
		}
		
		$crc = false;
		while(!$crc){
			$t1 = rand(100000,999999);
			$t2 = rand(100000,999999);
			$secret = "flag is:#$t1$t2";
			if(crc8($secret) % 27 == 0)
				$crc = true;
		}
	}
	
	$sql = "INSERT INTO `notes`(`position`, `text`, `secret`) VALUES  (GeomFromText('POINT($latLng)'),'Secret here', '$secret');";
	try{
		$res = $pdo->query($sql);
		echo "$latLng = $secret\r\n";
		if($i == ID_SECRET){
			echo "SECRET DONE HERE\r\n";
			sleep(2);
		}
		$i++;
	} catch(Exception $e) {
		echo "sql:".$sql." error:".$e;
	}
}