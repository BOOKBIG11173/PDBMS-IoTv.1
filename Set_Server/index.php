<?php 

    date_default_timezone_set('Asia/Bangkok');
    include_once("Firebase.php");
    include_once("Database.php");

    $firebase_url = "https://????????????????"; //Firebase = url firebase
    $firebase = new Firebase($firebase_url);
    $DB = new Database('localhost','????????','????????','?????????');      //PHPmyadmin = host,username,password,database_name
  
    if(!empty($_POST['id'])){

        $arr = explode(',',$_POST['id']);
        $data = ['id'=>$arr[0],'temp'=>$arr[1],'humid'=>$arr[2]];
        $date = date("d-m-Y");
        $time = date("H:i:s");

        $f_r =  $firebase->write("data/".$date."/".$time,$data);
        $db_r = $DB->insert('data',$data);

        echo json_encode(['firebase'=>$f_r,'sql'=>$db_r]);

    }

    if(!empty($_POST['get_data'])){
        $data = $DB->get('humid');
        echo json_encode($data);
    }

    if(empty($_POST)){
        header('Location:./home/');
        exit;
    }
?>
