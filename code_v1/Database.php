<?php

class Database {

    private $host = "";
    private $username = "";
    private $password = "";
    private $db_name = "";

    function __construct($host,$username,$password,$db_name) {
        $this->host = $host;
        $this->username = $username;
        $this->password = $password;
        $this->db_name = $db_name;
    }

    function insert($table,$data){
        $con = mysqli_connect($this->host , $this->username , $this->password , $this->db_name);
        mysqli_set_charset($con,"utf8");
        $sql = "insert into ".$table." values('".$data["id"]."','".$data["temp"]."','".$data["humid"]."','".date("Y-m-d H:i:s")."')";
        $save = mysqli_query($con,$sql);
        $con->close();
        return $save;
    }

    function get($table){
        $con = mysqli_connect($this->host , $this->username , $this->password , $this->db_name);
        mysqli_set_charset($con,"utf8");
        $sql = "select * from ".$table;
	$data=[];
        $get =  mysqli_query($con,$sql);
	while($r = mysqli_fetch_object($get)){
		$data[]=$r;
	}
        $con->close();
    //    return mysqli_fetch_object($get);
    	return $data;
	}
}
?>
