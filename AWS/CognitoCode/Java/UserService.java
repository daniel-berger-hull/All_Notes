package ca.gc.cbsa.cognito;

import java.util.Arrays;
import java.util.List;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;

import com.amazonaws.auth.AWSStaticCredentialsProvider;
import com.amazonaws.auth.AnonymousAWSCredentials;
import com.amazonaws.regions.Regions;
import com.amazonaws.services.cognitoidp.AWSCognitoIdentityProvider;
import com.amazonaws.services.cognitoidp.AWSCognitoIdentityProviderClientBuilder;
import com.amazonaws.services.cognitoidp.model.AttributeType;
import com.amazonaws.services.cognitoidp.model.ConfirmSignUpRequest;
import com.amazonaws.services.cognitoidp.model.ConfirmSignUpResult;
import com.amazonaws.services.cognitoidp.model.SignUpRequest;
import com.amazonaws.services.cognitoidp.model.SignUpResult;
import com.amazonaws.util.Base64;

public class UserService {
	
	
	final static public String TEST_USER_DANIEL_BERGER = "dberger";
	final static public String TEST_EMAIL_DANIEL_BERGER = "daniel.berger.hull@gmail.com";
	
	
	private String region = "ca-central-1";

	
/**  			COGNITO USER POOL ID    
   Can be found in the AWS Cognito Console
   - General settings (from the left hand menu)
   - Item Pool Id    (first item on top of the screen)
**/
//	private String poolId = "ca-central-1_B5ZATsu0p";
	private String poolId = "ca-central-1_faWjaRVGr";      //  MCOE ShuttleUsers
//	private String poolId = "ca-central-1_0WySe0ElU";		// CBSAMobileBorderPool
	
	
	/**  			COGNITO USER POOL : App Client ID  
	   Can be found in the AWS Cognito Console
	   - Select link App Clients        (from the left hand menu)
	   - Locate the App Client section  (There may be more than on App Client section for a user pool)
	   - Click on the button Show Details
	   --> App client id  & App client secret can be seen
	**/				 
	private String appClientId = "7a38mp9u1vrb3am26r3mrji7ii";								     // MCOE shuttlebug-android
	private String appClientSecretKey = "1o69m5u6rodf1tuglkgdb5rqj39qe48pegiphc2cburbmn3q52dj";  // MCOE shuttlebug-android
	
										 
	
	
	/*************************************************************************************
	 *                        Step #1 Signup
	 *  Can be run only if the use you want to create is not in the Cognito User Pool
	 *  Double check, from the  AWS Cognito Console, under the link User and Groups
	 *
	 *  Note 1: Some pool won't let the user register themselves.
	 *          Double check, under the link Policies
	 *          Radio button 'Allow users to sign themselves up' must be selected
	 *  
	 *  Note 2: Pay attention to the attributes that a User Pool accept when creating a new user
	 *          Some will accept only an email, other a userid + email, etc...
	 *          Double check, under the link Attributes
	 *************************************************************************************/
	public void signUp() {
		
		
		AnonymousAWSCredentials awsCreds = new AnonymousAWSCredentials();
		
		
		AWSStaticCredentialsProvider provide = new AWSStaticCredentialsProvider(awsCreds);
		
		
		AWSCognitoIdentityProvider cognitoIdentityProvider = AWSCognitoIdentityProviderClientBuilder.standard()
				.withCredentials(new AWSStaticCredentialsProvider(awsCreds)).withRegion(Regions.fromName(region))
				.build();

		SignUpRequest signUpRequest = new SignUpRequest().withClientId(appClientId)
				.withSecretHash(getSecretHash(TEST_USER_DANIEL_BERGER, appClientId, appClientSecretKey))
				.withPassword("Beer1212!")
				.withUsername(TEST_USER_DANIEL_BERGER);
		
		
//		List<AttributeType> attributeDataTypes = Arrays.asList(
//				new AttributeType().withName("email").withValue(TEST_EMAIL_DANIEL_BERGER),
//				new AttributeType().withName("name").withValue("Daniel Berger"));
		List<AttributeType> attributeDataTypes = Arrays.asList( new AttributeType().withName("email").withValue(TEST_EMAIL_DANIEL_BERGER) );
		
		
		signUpRequest.setUserAttributes(attributeDataTypes);

		try {
			SignUpResult result = cognitoIdentityProvider.signUp(signUpRequest);
			System.out.println(result);
		} catch (Exception e) {
			System.out.println(e);
		}
	}

	/*************************************************************************************
	 *                        Step #2 Signup - Code Verification   (optional)
	 *  Some user pool will send a confirmation on the user's email
	 *  Double check, from the  AWS Cognito Console, under the link 'Message customizations' 
	 *  the option 'Verification type' must be set to 'Code'
	 *  (Otherwise, a link is sent to the user, and we only need to click on 
	 *  it to activate the user)
	 *************************************************************************************/
	public boolean verifyAccessCode(String username, String code) {
		AnonymousAWSCredentials awsCreds = new AnonymousAWSCredentials();
		AWSCognitoIdentityProvider cognitoIdentityProvider = AWSCognitoIdentityProviderClientBuilder.standard()
				.withCredentials(new AWSStaticCredentialsProvider(awsCreds)).withRegion(Regions.fromName(region))
				.build();

		ConfirmSignUpRequest confirmSignUpRequest = new ConfirmSignUpRequest();
		confirmSignUpRequest.setUsername(username);
		confirmSignUpRequest.setConfirmationCode(code);
		confirmSignUpRequest.setClientId(appClientId);
		confirmSignUpRequest.setSecretHash((getSecretHash(TEST_USER_DANIEL_BERGER, appClientId, appClientSecretKey)));

		System.out.println("username=" + username);
		System.out.println("code=" + code);
		System.out.println("clientid=" + appClientId);

		try {
			ConfirmSignUpResult confirmSignUpResult = cognitoIdentityProvider.confirmSignUp(confirmSignUpRequest);
			System.out.println("confirmSignupResult=" + confirmSignUpResult.toString());
		} catch (Exception ex) {
			System.out.println(ex);
			return false;
		}
		return true;
	}

	private String getSecretHash(String userName, String clientId, String secretyKey) {

		try {
			Mac sha256_HMAC = Mac.getInstance("HmacSHA256");
			SecretKeySpec secret_key = new SecretKeySpec(secretyKey.getBytes(), "HmacSHA256");
			sha256_HMAC.init(secret_key);
			String payload = userName + clientId;
			String hash = Base64.encodeAsString(sha256_HMAC.doFinal(payload.getBytes()));
			System.out.println(hash);
			return hash;
		} catch (Exception e) {
			throw new RuntimeException(e);
		}

	}

	
	/*************************************************************************************
	 *                        Step #3 Validate user 
	 *  Can be done on an existing user, or a new user that we just confirmed the code or link
	 *  This code, if sucessfull, returns an Authoriser token
	 * 
	 *************************************************************************************/
	public String validateUser(String username, String password) {
		AuthenticationHelper helper = new AuthenticationHelper(poolId, appClientId, appClientSecretKey);
		return helper.PerformSRPAuthentication(username, password);
	}
	

	public static void main(String[] args) {

		UserService service = new UserService();
//		service.signUp();
//		//service.verifyAccessCode(TEST_USER_DANIEL_BERGER, "471370");
//		System.out.println(service.validateUser(TEST_USER_DANIEL_BERGER, "Beer1212!"));  
		
	}

}
