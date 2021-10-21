<?php
	if ($_SERVER['REQUEST_METHOD'] == 'POST') 
	{
		$directory = "./html/" . htmlspecialchars($_POST['my_directory']) . "/";
		
		if (!file_exists($directory))
		{
			echo 'Chemin invalide<br />';
			exit;
		}
		
		if (is_uploaded_file($_FILES['my_upload']['tmp_name'])) 
		{ 
			//First, Validate the file name
			if(empty($_FILES['my_upload']['name']))
			{
				echo "Name est vide";
				exit;
			}
			
			$upload_file_name = $_FILES['my_upload']['name'];
			//Too long file name?
			if(strlen ($upload_file_name)>100)
			{
				echo "Nom  de fichier trop long";
				exit;
			}
			
			//replace any non-alpha-numeric cracters in th file name
			$upload_file_name = preg_replace("/[^A-Za-z0-9 \.\-_]/", '', $upload_file_name);
			
			//set a limit to the file upload size
			if ($_FILES['my_upload']['size'] > 99999999999999999999) 
			{
				echo " Fichier trop gros ";
				exit;        
			}
			
			//Save the file
			$dest=$directory . $upload_file_name;
			if (!file_exists($dest))
			{
				if (move_uploaded_file($_FILES['my_upload']['tmp_name'], $dest)) 
				{
					echo 'Le fichier a ete uploader!<br />';
				}
			}
			else
			{
				echo 'Le fichier exist deja<br />';
			}
		}
	}
?>