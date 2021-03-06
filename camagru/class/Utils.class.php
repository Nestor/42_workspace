<?PHP
class Utils {
	const VALID_TYPE = 0;
	const FORGOT_TYPE = 1;
	const NEW_COMMENT = 2;

	// Function used to generate a pseudo random id (type uuid)
	public static function gen_uuid() {
		return sprintf('%04x%04x-%04x-%04x-%04x-%04x%04x%04x',
			mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff),
			mt_rand(0, 0x0fff) | 0x4000, mt_rand(0, 0x3fff) | 0x8000,
			mt_rand(0, 0xffff), mt_rand(0, 0xffff), mt_rand(0, 0xffff));
	}

	// Function used to verify that an string is a valid uuid
	public static function is_uuid( $uuid ) {
		if (!is_string($uuid) || (preg_match('/^[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$/', $uuid) !== 1) || strlen($uuid) != 36)
		    return false;
		else
			return true;
	}

	// Function is used to generate a random string of X length
	public static function random_string( $length ) {
    	$chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    	return substr(str_shuffle($chars), 0, $length);
	}

	// function used to send a mail to an user
	public static function sendMail($type, $user, $other = null) {
		switch($type) {
			case Utils::VALID_TYPE : {
				// generate a token and save it
				$token = Utils::gen_uuid();
				$db = Database::getInstance();
				$stmt = $db->prepare("INSERT INTO tokens (id, user, type) VALUES (?, ?, ?)");
				$stmt->execute(array($token, $user->id, "REGISTER"));

				// get template and replace variable
				$content = file_get_contents('./mail_template/register.html');
				$content = preg_replace("/%name%/", $user->name, $content);
				$content = preg_replace("/%url%/", "http://" . $_SERVER['HTTP_HOST'] . "/api/mail.php?type=valid&code=" . $token, $content);
				// send mail
				mail($user->mail, "[Instagrume] Valid your account to use our site !", $content, "From: noreply@instagrume.com\r\nContent-type:text/html;charset=UTF-8\r\n");
				break ;
			}
			case Utils::FORGOT_TYPE : {
				// get template and replace variable
				$content = file_get_contents('./mail_template/forgot.html');
				$content = preg_replace("/%name%/", $user->name, $content);
				$content = preg_replace("/%password%/", $other, $content);
				// send mail
				mail($user->mail, "[Instagrume] Your new password is here !", $content, "From: noreply@instagrume.com\r\nContent-type:text/html;charset=UTF-8\r\n");
				break ;
			}
			case Utils::NEW_COMMENT : {
				// get template and replace variable
				$content = file_get_contents('./mail_template/new_comment.html');
				$content = preg_replace("/%name%/", $user->name, $content);
				$user = User::query($other->author);
				$content = preg_replace("/%user_name%/", $user->name, $content);
				$content = preg_replace("/%user_comment%/", $other->content, $content);
				$content = preg_replace("/%url%/", "http://" . $_SERVER['HTTP_HOST'] . "/post.php#" . $other->post, $content);

				// send mail
				mail($user->mail, "[Instagrume] A user has commented your post, check it out !", $content, "From: noreply@instagrume.com\r\nContent-type:text/html;charset=UTF-8\r\n");
			}
		}
	}

	public static function imagecopymerge_alpha($dst_im, $src_im, $dst_x, $dst_y, $src_x, $src_y, $src_w, $src_h, $pct) {
		$cut = imagecreatetruecolor($src_w, $src_h);
		imagecopy($cut, $dst_im, 0, 0, $dst_x, $dst_y, $src_w, $src_h);
		imagecopy($cut, $src_im, 0, 0, $src_x, $src_y, $src_w, $src_h);
		imagecopymerge($dst_im, $cut, $dst_x, $dst_y, 0, 0, $src_w, $src_h, $pct);
	}

	// This functions is used to merge two image
	public static function mergeImage($src_a, $src_b) {
		$a = $src_a;
		$b = $src_b;
		$a = imagecreatefromstring($a);
		$b = imagecreatefromstring($b);
		$b = imagescale($b, imagesx($a) / 4);
		Utils::imagecopymerge_alpha($a, $b, imagesx($a) / 2 - imagesx($b) / 2, imagesy($a) / 100 * 10, 0, 0, imagesx($b), imagesy($b), 100);
		imagesavealpha($a, true);
		// php u fuckng suck
		ob_start();
		imagepng($a);
		$contents =  ob_get_contents();
		ob_end_clean();
		//
		imagedestroy($a);
		imagedestroy($b);
		return $contents;
	}

	// This function is used to verify that the string is valid base64
	public static function is_base64($s){
	    // Check if there are valid base64 characters
	    if (!preg_match('/^[a-zA-Z0-9\/\r\n+]*={0,2}$/', $s)) return false;

	    // Decode the string in strict mode and check the results
	    $decoded = base64_decode($s, true);
	    if(false === $decoded) return false;

	    // Encode the string again
	    if(base64_encode($decoded) != $s) return false;

	    return true;
	}

}
?>
