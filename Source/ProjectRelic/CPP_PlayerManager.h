// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_CharacterManager.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
#include "Components/ChildActorComponent.h"
#include "CPP_WeaponManager.h"

#include "CPP_PlayerManager.generated.h"

/**************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_PlayerManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Create the player character (Version 2 and above of PlayerManager)
 *
 * Functions: virtual void BeginPlay() override,
 *			  virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override
 *
 * References: N/A
 *
 * See Also: PlayerManager.h
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 15/03/2022    JA			 v1.0        Prototype version
 * 11/04/2022	 JA			 v1.1		 Melee and cover
 * 25/04/2022	 JA			 v1.2		 Adding invisiblity
 * 02/05/2022    JA          v2.0        Created a better PlayerManager
 * 30/05/2022	 JA			 v2.1		 Stealth Takedown
 * 30/05/2022	 JA			 v2.2		 Projectiles and Health
**************************************************************************************************************/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams( FPlayerUI, float, healthBar, float, invisibilityBar, int, ammoCount, int, reserveCount, bool ,assaultRifle, bool, pistol, bool, throwable );

UCLASS()
class PROJECTRELIC_API ACPP_PlayerManager : public ACPP_CharacterManager
{
	GENERATED_BODY()
private:
	bool m_canTakedown; // If Player can takedown AI
	float m_shotDamage; // Damage per shot taken from AI
	float m_animPosition; // Set position of Player to be behind the AI
	float m_animCompletion; // How long it takes to compelete the animation montage
	bool m_invisibility; // Whether the player is invisibile or not
	float m_invisibilityPercent; // The amount of invisibility powerup left
	float m_chanceOfHit; // Chance to take damage
	FTimerHandle m_invisiblityTimer; // Timer handle for invisiblity
	int m_currentlyEquipped; // Currently equipped weapon
	TArray< UChildActorComponent*> m_weaponInventory; // Array for player weapons

	bool m_hitmarkerActive;

	FTimerHandle m_hitmarkerTimer;

	UMaterialInterface* m_material; // Material 
	UMaterialInterface* m_material1; // Material 
	UMaterialInterface* m_material2; // Material 
	UMaterialInterface* m_material3; // Material 
	UMaterialInterface* m_material4; // Material 
	UMaterialInterface* m_material5; // Material 
	UMaterialInterface* m_material6; // Material 
	UMaterialInterface* m_material7; // Material 
	UMaterialInterface* m_material8; // Material 
	UMaterialInterface* m_material9; // Material 
	UMaterialInterface* m_material10; // Material 


	UMaterialInstanceDynamic* m_dynamicMaterial; // Dynamic materialPlay

	FName m_pistolSocket;
	FName m_pistolMuzzleSocket;

	void InvisibilityTick( float DeltaTime );
	
	void InvisibilityEvaluation( float DeltaTime );
	
	void InvisibilityMaterial();

	void AmmoTick();
	void AmmoEvaluation( int ammoCount, int reserveCount, int fullMag );

	void HitmarkerFinished();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeTakedown")
		float takedownTraceDistance; // Raycast distance

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animations" )
		UAnimMontage* animTakedown; // Anim Montage for Player stealth takedown

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animations" )
		UAnimMontage* deadAnim; // Anim Montage for Player stealth takedown

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material" )
		UMaterial* invisibleMaterial;

	//UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material" )
		//URuntimeMeshComponent* playerMesh;

	UPROPERTY( EditAnywhere, Category = "Health" )
		float health; // Player health

	UPROPERTY( EditAnywhere, Category = "Health" )
		float defaultHealth; // Player default health

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UChildActorComponent* primaryGun;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UChildActorComponent* pistol;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SFX" )
		USoundBase* callEnemy;

	
public:
	ACPP_PlayerManager();
	/*****************************************************************************
	*   Function        : virtual void BeginPlay() override
	*   Purpose         : Begin event
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 02/05/2022 
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	virtual void BeginPlay() override;
	/*******************************************************************************************************************
	*   Function        : virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override
	*   Purpose         : Player inputs
	*   Parameters      : class UInputComponent* PlayerInputComponent
	*   Returns         : N/A
	*   Date altered    : 02/05/2022 
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*******************************************************************************************************************/
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	/*****************************************************************************
	 *   Function        : void SetCanTakedown( bool canTakedown )
	 *   Purpose         : Set bool for being able to stealth takedown AI
	 *   Parameters      : bool canTakedown
	 *   Returns         : N/A
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
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


	void SetCanTakedown( bool canTakedown );
	/*****************************************************************************
	 *   Function        : bool GetCanTakedown()
	 *   Purpose         : Get bool for being able to stealth takedown AI
	 *   Parameters      : N/A
	 *   Returns         : m_canTakedown
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	bool GetCanTakedown();
	/*****************************************************************************
	 *   Function        : void Takedown()
	 *   Purpose         : Stealth Takedown AI (The actual action)
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void Takedown();
	/*****************************************************************************
	 *   Function        : void TraceForward_Implementation()
	 *   Purpose         : Send a raycast to check if AI is infront
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void TraceForwardImplementation();
	/*****************************************************************************
	 *   Function        : void AnimationExecuted()
	 *   Purpose         : Actions after anim montage has been played
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void AnimationExecuted();
	/*****************************************************************************
	 *   Function        : virtual void TakeAttack() override
	 *   Purpose         : When Player has been hit by projectile
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : CPP_CharacterManager::ShootProjectile
	*****************************************************************************/
	virtual void TakeAttack() override;
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
	UFUNCTION( BlueprintCallable )
		void userInterfaceDelegate();

	UPROPERTY( BlueprintCallable, BlueprintAssignable )
		FPlayerUI userInterfaceD;
	
	void Invisibility();

	bool GetInvisibilityStatus();

	void InvisibilityFinished();

	void Respawn();

	UFUNCTION( BlueprintCallable )
		void IncreaseAmmoCount( int ammo );

	void EquipGun( TArray<UChildActorComponent*> WeaponInventory );

	void ChangeWeapons( float inputAxis );

	UFUNCTION( BlueprintCallable )
		bool GetAssaultRifle();

	UFUNCTION( BlueprintCallable )
		bool GetPistol();

	UFUNCTION( BlueprintCallable )
		bool GetThrowable();


	UFUNCTION( BlueprintCallable )
		bool GetHitmarkerActive();

	
		void SetHitmarkerActive( bool hitmarkerActive );
	void DistractEnemy();
};
