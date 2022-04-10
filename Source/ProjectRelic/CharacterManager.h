// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectileManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterManager.generated.h"

/***************************************************************************************
 * Special thank you to Joseph Gilmore for the headers <3 
 * 
 * 
 * Type: Class
 *
 * Name: CharacterManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Parent class for characters
 *
 * Functions: ACharacterManager(), ~ACharacterManager(), void Shoot(), 
 *			  void MoveForward(), void MoveRight(), void BeginSprint(),
 *			  void EndSprint(), void BeginCrouch(), void EndCrouch(), void AimIn(),
 *			  void AimOut(), void SetHoldADS(), bool GetHoldADS(),
 *			  virtual void Tick( float deltaTime ), 
 *			  virtual void SetupPlayerInputComponent()
 *
 * References: N/A
 *
 * See Also: PlayerCharacter, EnemyCharacter
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 12/03/2022    JA          1.0         Needs to be reworked, good for prototype
 ***************************************************************************************/

UCLASS()
class PROJECTRELIC_API ACharacterManager : public ACharacter
{
	GENERATED_BODY()

private:
	
	bool m_holdADS; // Toggle holding ADS setting
	float m_muzzleRotationPitch;
	float m_walkSpeed;
	float m_sprintSpeed;
	float m_crouchSpeed;

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

	// Muzzle offset
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
		FVector muzzleOffset;

protected:
	/********************************************************
	   *   Function        : virtual void BeginPlay() override
	   *   Purpose         : Called when the game starts or when spawned
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	virtual void BeginPlay() override;
	
	// Spring Arm Component to follow the camera behind the player
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USpringArmComponent* springArmComp;

	// Player follow camera
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UCameraComponent* cameraComp;

	// Player ADS camera
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UCameraComponent* aDSCameraComp;

	// Gun
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USkeletalMeshComponent* gunComp;

	UPROPERTY( EditDefaultsOnly, Category = Projectile )
		TSubclassOf<class AProjectileManager> projectileClass;

	/********************************************************
	   *   Function        : void MoveForward()
	   *   Purpose         :
	   *   Parameters      : float inputAxis
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	// Input 
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
	   *   Purpose         :
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	// Sprint
	void BeginSprint();
	/********************************************************
	   *   Function        : void EndSprint()
	   *   Purpose         :
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
	   *   Purpose         :
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	// Crouch
	void BeginCrouch();
	/********************************************************
	   *   Function        : void EndCrouch()
	   *   Purpose         :
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
	   *   Purpose         :
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
	   *   Purpose         :
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 15/03/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void AimOut();
	/********************************************************
	   *   Function        : void SetHoldADS()
	   *   Purpose         :
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
	   *   Purpose         :
	   *   Parameters      : N/A
	   *   Returns         : N/A
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
public:
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
	virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override;


};
