<!DOCTYPE HTML>  
<html>
<head>
<style>
.error {color: #FF0000;}
</style>
</head>
<body>  

<?php
// define variables and set to empty values
$nameErr = $uidErr = $genderErr = "";
$name = $username = $gender = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
  if (empty($_POST["name"])) {
    $nameErr = "Name is required";
  } else {
      $name = test_input($_POST["name"]);
      if (!preg_match ("/^[a-zA-Z ]*$/",$name)) {
          $nameErr = "Only letters and white space allowed";
      }
  }
  
  if (empty($_POST["uid"])) {
    $uidErr = "Username is required";
  } else {
    $uid = test_input($_POST["uid"]);
      if (!preg_match ("/^[a-zA-Z0-9 ]*$/",$name)) {
          $nameErr = "Only letters, numbers and white space allowed";
      }
  }  
  if (empty($_POST["gender"])) {
    $genderErr = "Gender is required";
  } else {
    $gender = test_input($_POST["gender"]);
  }
}

function test_input($data) {
  $data = trim($data);
  $data = stripslashes($data);
  $data = htmlspecialchars($data);
  return $data;
}
?>

<h2>Drunk Driving</h2>

<?php echo "Current time: " .date("h:i:sa"); "<br><br>"; ?>
<p><span class="error">* required field.</span></p>
<form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>">  
  Name: <input type="text" name="name">
  <span class="error">* <?php echo $nameErr;?></span>
  <br><br>
  Username: <input type="text" name="uid">
  <span class="error">* <?php echo $uidErr;?></span>
  <br><br>
  Gender:
  <input type="radio" name="gender" value="female">Female
  <input type="radio" name="gender" value="male">Male
  <span class="error">* <?php echo $genderErr;?></span>
  <br><br>
  <input type="submit" name="submit" value="Submit">  
</form>

<?php
echo "<h2>Input:</h2>";
echo $name;
echo "<br>";
echo $uid;
echo "<br>";
echo "<br>";
echo $gender;
?>

</body>
</html>
