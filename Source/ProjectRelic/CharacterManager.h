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
	// Toggle holding ADS setting
	bool m_holdADS;

public:
	/********************************************************
	   *   Function        : ACharacterManager()
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
    *********************************************************/
	ACharacterManager();
	/********************************************************
	   *   Function        : void Shoot()
	   *   Purpose         : Fire projectile
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
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
	   *   Date altered    : 12/03/2022
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
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
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
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	// Sprint
	void BeginSprint();
	/********************************************************
	   *   Function        : void EndSprint()
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	void EndSprint();
	/********************************************************
	   *   Function        : void BeginCrouch()
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	// Crouch
	void BeginCrouch();
	/********************************************************
	   *   Function        : void EndCrouch()
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	void EndCrouch();
	/********************************************************
	   *   Function        : void AimIn()
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	void AimIn();
	/********************************************************
	   *   Function        : void AimOut()
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	void AimOut();
	/********************************************************
	   *   Function        : void SetHoldADS()
	   *   Purpose         :
	   *   Parameters      : bool holdADS 
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	void SetHoldADS( bool holdADS );
	/********************************************************
	   *   Function        : bool GetHoldADS()
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	bool GetHoldADS();

public:
	/*********************************************************************
	   *   Function        : virtual void Tick( float deltaTime ) override
	   *   Purpose         : Called every frame
	   *   Parameters      : float deltaTime
	   *   Returns         : N/A
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	***********************************************************************/
	virtual void Tick( float deltaTime ) override;
	/**********************************************************************************************************************
	   *   Function        : virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override
	   *   Purpose         : Called to bind functionality to input
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	***********************************************************************************************************************/
	virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override;


};
