// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CPP_Projectile.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ChildActorComponent.h"
#include "CPP_AssaultRifle.h"
#include "Camera/CameraShakeBase.h"
#include "Components/AudioComponent.h"
#include "CPP_CharacterManager.generated.h"
/**************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_CharacterManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Parent class for characters (Version 2 and above of the CharacterManager)
 *
 * Functions: ACPP_CharacterManager() virtual void BeginPlay() override,
 *			  virtual void Tick( float DeltaTime ) override,
 *			  virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override,
 *			  void UpdateWalkSpeed( float speed ), class USpringArmComponent* springArmComp,
 *			  void MoveForward( float inputAxis ),
 *			  void MoveRight( float inputAxis ), void BeginSprint(), void EndSprint(),
 *			  void BeginCrouch(), void EndCrouch(),
 *	
 * References: N/A
 *
 * See Also: CharacterManager.h
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 21/03/2022    JA          v1.0        Needs to be reworked, good for prototype
 * 28/03/2022    JA			 v1.1		 Added bullets
 * 18/04/2022	 JA			 v1.2		 Melee takedown features
 * 02/05/2022    JA          v2.0        Created a better CharacterManager - Features need to be reimplemented
 * 30/05/2022    JA			 v2.1		 Changed physic bullets to raycast
 **************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_CharacterManager : public ACharacter
{
	GENERATED_BODY()

private:
	bool m_isCrouched; // If character is crouched
	float m_muzzleRotationPitch; // Muzzle rotation
	float m_weaponRange; // The weapon range
	bool m_isInCover; // If character is in cover
	FTimerHandle m_reloadTime; // Time handle for reloading
	float m_reloadAnimTime; // Animation time for reloading
	int m_fullMag; // Amount the magazine can hold

	bool m_isShooting; // If character is shooting

	FTimerHandle m_hitmarkerTimer;
protected:
	int m_ammoCount; // The amount of ammunition in the main tank
	int m_reserveAmmo; // Amount of ammunition in the reserve tank
	bool m_assaultRifle; // If AR is active
	bool m_pistol; // If pistol is active
	bool m_shotInHead; // If enemy has been shot in the head
	
	bool m_hitmarker;
public:
	bool m_aimingIn;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USpringArmComponent* springArmComp; // Spring Arm Component to follow the camera behind the player

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UCameraComponent* cameraComp; // Player follow camera

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USkeletalMeshComponent* gunComp; // Mesh for gun

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UStaticMeshComponent* bulletComp; // Mesh for gun

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USphereComponent* headCollision;

	UPROPERTY( EditAnywhere, Category = "Projectiles" )
		float m_projectileRange; // Range for projectile ray cast

	UPROPERTY( EditAnywhere, Category = "Projectiles" )
		float recoil; // Recoil value

	UPROPERTY( EditAnywhere, Category = "Projectiles" )
		bool m_canBeShot; // If character can be shot and destroyed by projectile

	UPROPERTY( EditDefaultsOnly, Category = Projectile )
		TSubclassOf<class ACPP_Projectile> projectileClass; // Projectile class

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Projectiles" )
		FVector m_muzzleOffset; // Muzzle offset

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Projectiles" )
		float timeBetweenShots; // Seconds between shots

	/*UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Projectiles" )
		UCameraShakeBase* cameraShake;*/

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animations" )
		UParticleSystem* animShoot; // Anim Montage for Player stealth takedown

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animations" )
		UAnimMontage* animReload; // Anim Montage for Player stealth takedown

	

	FName weaponSocket;
	FName muzzleSocket;
	FName headSocket;
public:
	/*****************************************************************************
	 *   Function        : ACPP_CharacterManager()
	 *   Purpose         : Constructor
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 02/05/2022 
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	ACPP_CharacterManager();
	/*****************************************************************************
	 *   Function        : virtual void BeginPlay() override
	 *   Purpose         : BeginPlay event
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 02/05/2022 
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	virtual void BeginPlay() override;
	/*****************************************************************************
		 *   Function        : virtual void Tick( float DeltaTime ) override
		 *   Purpose         : Event Tick
		 *   Parameters      : float DeltaTime
		 *   Returns         : N/A
		 *   Date altered    : 02/05/2022 
		 *   Contributors    : Jaber Ahmed
		 *   Notes           : N/A
		 *   See also        : N/A
		*****************************************************************************/
	virtual void Tick( float DeltaTime ) override;
	/************************************************************************************************************************
		 *   Function        : virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override
		 *   Purpose         : Character inputs
		 *   Parameters      : class UInputComponent* PlayerInputComponent
		 *   Returns         : N/A
		 *   Date altered    : 02/05/2022 
		 *   Contributors    : Jaber Ahmed
		 *   Notes           : N/A
		 *   See also        : N/A
	************************************************************************************************************************/
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	/*****************************************************************************
	 *   Function        : void UpdateWalkSpeed( float speed )
	 *   Purpose         : Speed up or slow down character
	 *   Parameters      : float speed 
	 *   Returns         : N/A
	 *   Date altered    : 02/05/2022 
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void UpdateWalkSpeed( float speed );
	/*****************************************************************************
	 *   Function        : void MoveForward( float inputAxis )
	 *   Purpose         : Make character move forward
	 *   Parameters      : float inputAxis
	 *   Returns         : N/A
	 *   Date altered    : 02/05/2022 
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void MoveForward( float inputAxis );
	/*****************************************************************************
	 *   Function        : void MoveRight( float inputAxis )
	 *   Purpose         : Make character move right
	 *   Parameters      : float inputAxis
	 *   Returns         : N/A
	 *   Date altered    : 02/05/2022 
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void MoveRight( float inputAxis );
	/*****************************************************************************
	 *   Function        : void BeginSprint()
	 *   Purpose         : Make character sprint
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 02/05/2022 
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void BeginSprint();
	/*****************************************************************************
	 *   Function        : void EndSprint()
	 *   Purpose         : Stop character sprinting
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 02/05/2022 
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void EndSprint();
	/*****************************************************************************
	 *   Function        : void BeginCrouch()
	 *   Purpose         : Make character crouch
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 02/05/2022 
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void BeginCrouch();
	/*****************************************************************************
	 *   Function        : void EndCrouch()
	 *   Purpose         : Stop character crouching
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 02/05/2022 
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void EndCrouch();
	 /*****************************************************************************
	  *   Function        : FHitResult RaycastShot()
	  *   Purpose         : Send raycast which portrays a projectile being shot
	  *   Parameters      : N/A
	  *   Returns         : hit
	  *   Date altered    : 30/05/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A           
	  *****************************************************************************/
	FHitResult RaycastShot();
	/*****************************************************************************
	  *   Function        : void ShootProjectile()
	  *   Purpose         : Call to shoot projectile
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 30/05/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	UFUNCTION( BlueprintCallable, Category = "Projectiles" )
		void ShootProjectile();
	/*********************************************************************************************************
	  *   Function        : virtual void TakeAttack()
	  *   Purpose         : When Character has been hit by projectile
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 30/05/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : This function can be overriden for desired effect on specific characters and pawns
	  *   See also        : CPP_PlayerManager::TakeAttack, CPP_AIManager::TakeAttack
	 *********************************************************************************************************/
	virtual void TakeAttack();
	/*****************************************************************************
	  *   Function        : void 
	  *   Purpose         : 
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	bool GetIsCrouched();
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void StartShooting();
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void StopShooting();
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void StartAim();
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void StopAim();

	FTimerHandle m_shootTime;
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void Reloaded();
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void StartCover( FHitResult hit );
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void StopCover();
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void WallTrace();
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void TakeCover();
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	bool CoverTrace( float inputAxis );
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	bool RightCoverTrace();
	/*****************************************************************************
	  *   Function        : void
	  *   Purpose         :
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	bool LeftCoverTrace();

	void EnemyShoot();

	UFUNCTION( BlueprintCallable )
		bool GetIsAimedIn();

	void HasBeenShotInTheHead( bool boolean );
	void HitmarkerFinish();
};
