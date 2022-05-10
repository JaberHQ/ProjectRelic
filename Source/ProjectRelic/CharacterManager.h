// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectileManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "Math/Vector.h"
#include "CharacterManager.generated.h"

/******************************************************************************************************
 * Special thank you to Joseph Gilmore for the headers <3 
 * 
 * Type: Class
 *
 * Name: CharacterManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Parent class for characters (Prototype)
 *
 * Functions: ACharacterManager(), ~ACharacterManager(), void Shoot(), 
 *			  void MoveForward(), void MoveRight(), void BeginSprint(),
 *			  void EndSprint(), void BeginCrouch(), void EndCrouch(), void AimIn(),
 *			  void AimOut(), void SetHoldADS(), bool GetHoldADS(),
 *			  virtual void Tick( float deltaTime ), 
 *			  virtual void SetupPlayerInputComponent(), void TimerFunction(), 
 *			  void LineTrace(), void TakedownTrace(), void UpdateWalkSpeed( float speed )
 *
 * References: N/A
 *
 * See Also: PlayerCharacter, EnemyCharacter
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 21/03/2022    JA          v1.0        Needs to be reworked, good for prototype
 * 28/03/2022    JA			 v1.1		 Added bullets
 * 18/04/2022	 JA			 v1.2		 Melee takedown features
 * 02/04/2022	 JA			 v1.21		 Depreciated, will be replaced with CPP_CharacterManager
******************************************************************************************************/

UCLASS()
class PROJECTRELIC_API ACharacterManager : public ACharacter
{
	GENERATED_BODY()

private:
	bool m_holdADS; // Toggle holding ADS setting
	float m_muzzleRotationPitch; // Muzzle rotation
	float m_walkSpeed; // Walk speed
	float m_sprintSpeed; // Sprint speed
	float m_crouchSpeed; // Crouch speed

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USpringArmComponent* springArmComp; // Spring Arm Component to follow the camera behind the player
	
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UCameraComponent* cameraComp; // Player follow camera
	
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UCameraComponent* aDSCameraComp; // Player ADS camera
	
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USkeletalMeshComponent* gunComp; // Gun
	
	UPROPERTY( EditDefaultsOnly, Category = Projectile )
		TSubclassOf<class AProjectileManager> projectileClass; // Projectile class

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
		FVector muzzleOffset; // Muzzle offset

	FTimerHandle timerHandle; // Timer

public:
	/********************************************************
	   *   Function        : ACharacterManager()
	   *   Purpose         : Constructor
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
    *********************************************************/
	ACharacterManager();
	/***********************************************************
	   *   Function        : void Shoot()
	   *   Purpose         : Call function to shoot projectiles
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	***********************************************************/
	UFUNCTION()
		void Shoot();
	/*******************************************************************
	   *   Function        : virtual void BeginPlay() override
	   *   Purpose         : Called when the game starts or when spawned
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	********************************************************************/
	virtual void BeginPlay() override;
	/********************************************************
	   *   Function        : void MoveForward()
	   *   Purpose         : Allow player to move forward
	   *   Parameters      : float inputAxis
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/ 
	void MoveForward( float inputAxis );
	/********************************************************
	   *   Function        : MoveRight()
	   *   Purpose         : Allow character to move right
	   *   Parameters      : float inputAxis
	   *   Returns         : N/A
	   *   Date altered    : 12/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void MoveRight( float inputAxis );
	/********************************************************
	   *   Function        : void BeginSprint()
	   *   Purpose         : Start sprinting
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void BeginSprint();
	/********************************************************
	   *   Function        : void EndSprint()
	   *   Purpose         : Stop sprinting
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void EndSprint();
	/********************************************************
	   *   Function        : void BeginCrouch()
	   *   Purpose         : Start crouching
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void BeginCrouch();
	/********************************************************
	   *   Function        : void EndCrouch()
	   *   Purpose         : Stop crouching
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void EndCrouch();
	/********************************************************
	   *   Function        : void AimIn()
	   *   Purpose         : Aim gun in
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void AimIn();
	/********************************************************
	   *   Function        : void AimOut()
	   *   Purpose         : Aim gun out
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void AimOut();
	/********************************************************
	   *   Function        : Takedown
	   *   Purpose         : Melee takedown the enemy
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 18/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	virtual void Takedown();
	/********************************************************
	   *   Function        : void SetHoldADS()
	   *   Purpose         : Setting to toggle holding of ADS
	   *   Parameters      : bool holdADS 
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void SetHoldADS( bool holdADS );
	/********************************************************
	   *   Function        : bool GetHoldADS()
	   *   Purpose         : Get if ADS is held or pressed
	   *   Parameters      : N/A
	   *   Returns         : return m_holdADS;
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	bool GetHoldADS();
	/****************************************************************
	   *   Function        : void UpdateWalkSpeed( float chaseSpeed )
	   *   Purpose         : Increase or decrease enemy walk speed
	   *   Parameters      : float chaseSpeed
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*****************************************************************/
	void UpdateWalkSpeed( float speed );
	/**********************************************************************
	   *   Function        : void TakedownTrace();
	   *   Purpose         : Line to check if enemy is infront for takedown
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 18/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	**********************************************************************/
	void TakedownTrace();
	/*********************************************************************
	   *   Function        : virtual void Tick( float deltaTime ) override
	   *   Purpose         : Called every frame
	   *   Parameters      : float deltaTime
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	***********************************************************************/
	virtual void Tick( float deltaTime ) override;
	/**********************************************************************************************************************
	   *   Function        : virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override
	   *   Purpose         : Called to bind functionality to input
	   *   Parameters      : class UInputComponent* playerInputComponent
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	***********************************************************************************************************************/
	virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent );
	/*********************************************************************
	   *   Function        : virtual void Tick( float deltaTime ) override
	   *   Purpose         : Called every frame
	   *   Parameters      : float deltaTime
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	***********************************************************************/
	void LineTrace();
	/*****************************************************************************
	  *   Function        : void TimerFunction()
	  *   Purpose         : Set timer
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 18/04/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	*****************************************************************************/
	void TimerFunction();
	
};
