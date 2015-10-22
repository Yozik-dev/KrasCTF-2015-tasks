<?php

class HttpGetter
{
    const TIMEOUT = 30;

    public static function Get($default_url, $data = array(), $port = 80, $timeout = self::TIMEOUT)
    {
        $url = parse_url($default_url);
        if(!isset($url['host']) || $url['host'] == ''){
			return false;
		}
		if(!isset($url['scheme']) || $url['scheme'] != 'http'){
			return false;
		}
		
        return file_get_contents($default_url);
    }

    public static function Post($default_url, $data = [])
    {
		$headers = array(
			"Content-type: application/json",
		);
		
		$data = json_encode($data);
			
        $myCurl = curl_init();
        curl_setopt_array($myCurl, array(
            CURLOPT_URL => $default_url,
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_HTTPHEADER => $headers,
            CURLOPT_ENCODING => 'gzip,deflate',
            CURLOPT_POST => true,
            CURLOPT_POSTFIELDS => $data
        ));
        $response = curl_exec($myCurl);
        curl_close($myCurl);
        return $response;
    }
}
