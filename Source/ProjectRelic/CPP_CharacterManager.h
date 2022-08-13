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
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "CPP_Throwable.h"
#include "CPP_Pistol.h"
#include "CPP_PredictionEndPoint.h"
#include "CPP_CharacterManager.generated.h"
/**************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_CharacterManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Parent class for characters (Player and AI in particular)
 * 
 * References: 
 *
 * See Also: CPP_PlayerManager, CPP_AIManager
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
	int m_fullMag; // Amount the magazine can hold
	float m_muzzleRotationPitch; // Muzzle rotation
	float m_weaponRange; // The weapon range
	FTimerHandle m_hitmarkerTimer; // Time handle for hitmarkers


protected:
	int m_ammoAR; // Ammo for AR
	int m_reserveAR; // Reserve ammo for AR
	int m_fullMagAR; // Full magazine for AR
	int m_ammoPistol; // Ammo for pistol
	int m_reservePistol; // Reserve ammo for pistol
	int m_fullMagPistol; // Full magazine for pistol
	int m_throwableAmount; // The ammount of throwables
	int m_splineIndex; // Prediction spline index
	float m_reloadAnimTime; // Animation time for reloading
	bool m_isShooting; // If character is shooting
	bool m_shotInHead; // If enemy has been shot in the head
	bool m_hitmarker; // Hitmarker active
	bool m_turnRight; // Turn right
	bool m_turnLeft; // Turn left
	bool m_isCrouched; // If character is crouched
	FTimerHandle m_reloadTime; // Time handle for reloading

public:
	float m_throwSpeed; // Throw speed
	TArray<FVector> pointLocation; // Point location
	FTimerHandle m_shootTime; // Time handle for shooting
	FName weaponSocket; // Weapon bone socket
	FName muzzleSocket; // Muzzle bone socket
	FName headSocket; // Head bone socket
	FName m_throwSocket; // Throw bone socket
	FName noiseTag; // Noise tag to report
	FName gunNoiseTag; // Gun noise tag to report

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Components" )
		class USpringArmComponent* springArmComp; // Spring Arm Component to follow the camera behind the player

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Components" )
		class UCameraComponent* cameraComp; // Player follow camera

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Components" )
		class USkeletalMeshComponent* gunComp; // Mesh for gun

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Components" )
		class UStaticMeshComponent* bulletComp; // Mesh for gun

	UPROPERTY( EditAnywhere, Category = "Projectiles" )
		float m_projectileRange; // Range for projectile ray cast

	UPROPERTY( EditAnywhere, Category = "Projectiles" )
		float recoil; // Recoil value

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Projectiles" )
		float timeBetweenShots; // Seconds between shots

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Projectiles" )
		FVector m_muzzleOffset; // Muzzle offset

	UPROPERTY( EditDefaultsOnly, Category = "Projectiles" )
		TSubclassOf<class ACPP_Projectile> projectileClass; // Projectile class

	UPROPERTY( EditAnywhere, Category = "Projectiles" )
		bool m_canBeShot; // If character can be shot and destroyed by projectile

	UPROPERTY( EditAnywhere, Category = "Weapon" )
		bool m_assaultRifle; // If AR is active in hand

	UPROPERTY( EditAnywhere, Category = "Weapon" )
		bool m_pistol; // If pistol is active in hand

	UPROPERTY( EditAnywhere, Category = "Weapon" )
		bool m_throwable; // If throwable is active in hand

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animations" )
		UParticleSystem* animShoot; // Anim Montage for Player stealth takedown

	

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SFX" )
		USoundBase* shootSFX; // Shot sound effect

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SFX" )
		USoundBase* pistolShootSFX; // Shot sound effect

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SFX" )
		USoundBase* emptyGunSFX; // Empty magazine sound effect
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Throwable" )
		USplineComponent* m_predictionSpline; // Prediction spline component

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Throwable" )
		TArray<USplineMeshComponent*> m_predictionSplineMesh; // Prediction spline mesh 

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Throwable" )
		ACPP_PredictionEndPoint* m_predictionEndPoint; // End point of prediction spline

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Throwable" )
		UStaticMesh* cylinderPredictionMesh; // Prediction spline mesh components

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Throwable" )
		USceneComponent* m_throwSceneComp; // Throw scene comp

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Throwable" )
		class UChildActorComponent* throwable; // Throwable child actor

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Weapon" )
		bool m_aimingIn; // Holding down sights

public:
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
	/*****************************************************************************
	 *   Function        : bool GetIsAimedIn()
	 *   Purpose         : Get bool
	 *   Parameters      : N/A
	 *   Returns         : m_aimingIn
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	UFUNCTION( BlueprintCallable )
		bool GetIsAimedIn();
	/*****************************************************************************
	 *   Function        : int GetSplineIndex()
	 *   Purpose         : Get spline index
	 *   Parameters      : N/A
	 *   Returns         : m_splineIndex
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	UFUNCTION( BlueprintCallable )
		int GetSplineIndex();
	
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
	  *   Function        : void Reloaded()
	  *   Purpose         : Reload gun
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void Reloaded();
	/*****************************************************************************
	  *   Function        : void TakeCover()
	  *   Purpose         : Take cover action
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void TakeCover();
	/*****************************************************************************
	 *   Function        : void EnemyShoot()
	 *   Purpose         : For AI to shoot projectile
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void EnemyShoot();
	/*****************************************************************************
	 *   Function        : void HasBeenShotInTheHead( bool boolean )
	 *   Purpose         : Damage multipler for shots to the skull
	 *   Parameters      : bool boolean 
	 *   Returns         : N/A
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void HasBeenShotInTheHead( bool boolean );
	/*****************************************************************************
	 *   Function        : void HitmarkerFinish()
	 *   Purpose         : When hitmarker timer has finshed 
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void HitmarkerFinish();
	/*****************************************************************************
	  *   Function        : void StopShooting()
	  *   Purpose         : Character to stop shooting
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void StopShooting();
	/*****************************************************************************
	  *   Function        : bool GetIsShooting();
	  *   Purpose         : Get if the character is shooting
	  *   Parameters      : N/A
	  *   Returns         : m_isShooting
	  *   Date altered    : 07/08/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/

	UFUNCTION( BlueprintCallable )
		bool GetIsShooting();
};
