<?php

class Firebase {
    
    private $url;

    function __construct($firebase_url){
        $this->url = $firebase_url;
    }

    function write($path="",$data=[])
    {
        $HTTP = [
            'http' => [
                'method'  => 'PUT',
                'header'  => 'Content-type: application/json',
                'content' => json_encode($data)]
        ];
        $context = stream_context_create($HTTP);
        $contents = file_get_contents($this->url."/".$path.".json", false, $context);
        return ($contents!=false)?true:false;
    }

    function read($path=""){
         $HTTP = [
            'http' => [
                'method'  => 'GET',
                'header'  => 'Content-type: application/json'
                ]
        ];
        $context = stream_context_create($HTTP);
        $contents = file_get_contents($this->url."/".$path.".json", false, $context);
        return $contents;
    }

}

?>