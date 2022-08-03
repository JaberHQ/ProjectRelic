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
 * References: https://docs.unrealengine.com/4.27/en-US/InteractiveExperiences/HowTo/CharacterMovement/
 *				https://www.youtube.com/watch?v=a5gA7q5GD7Q&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=1
 *				https://www.youtube.com/watch?v=HrIAbcixoiQ&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=5
 *				https://www.youtube.com/watch?v=bQtbhWKsQKA&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=7
 *				https://www.youtube.com/watch?v=UZYa21qs82w&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=15
 *				https://www.youtube.com/watch?v=09ZXNjX4xTk&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=16
 *				https://www.youtube.com/watch?v=FMcW7t5ivEg&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=17
 *				https://www.youtube.com/watch?v=nn057V9KQDs&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=18
 *				https://www.youtube.com/watch?v=wNDrCcjtLdA&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=19
 *				https://www.youtube.com/watch?v=qACoBfFIe7o&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=20
 *				https://www.youtube.com/watch?v=0ZvAnUTodfI&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=25
 *				https://www.youtube.com/watch?v=n0GoiyS6I-o&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=26
 *				https://www.youtube.com/watch?v=m4oSOugBQCg&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=28
 *				https://www.youtube.com/watch?v=-tM4sDpDgrE&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=34
 *				https://www.youtube.com/watch?v=ysqRp7oeZME&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=38
 *				https://www.youtube.com/watch?v=usd0U9B0wTw&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=39
 *				https://www.youtube.com/watch?v=-Lg-_EU6NoU&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=42
 *				
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
 * 06/06/2022    JA			 v2.3		 Player cover
 * 27/06/2022	 JA			 v2.4		 Switching weapons
 * 04/07/2022	 JA			 v2.5		 Invisibility
 * 11/07/2022	 JA			 v2.6		 Ammo count, pickups
 * 18/07/2022	 JA			 v2.7		 Something to throw
 * 25/07/2022	 JA			 v2.8	     HUD, bug fixes
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
	float m_invisibilityFull;
	float m_invisibilityTimeMultiplier;
	float m_invisibilityTimeDrain; // Multplier for how quick invisibility drains
	float m_chanceOfHit; // Chance to take damage
	FTimerHandle m_invisiblityTimer; // Timer handle for invisiblity


	TArray< UChildActorComponent*> m_weaponInventory; // Array for player weapons

	bool m_hitmarkerActive;
	bool m_deathHitmarkerActive;
	bool m_isInCover; // If character is in cover

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


private:
	/*****************************************************************************
	*   Function        : void InvisibilityTick( float DeltaTime )
	*   Purpose         : Tick for Player invisiblity
	*   Parameters      : float DeltaTime
	*   Returns         : N/A
	*   Date altered    : 03/08/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void InvisibilityTick( float DeltaTime );
	/*****************************************************************************
	*   Function        : void InvisibilityEvaluation( float DeltaTime )
	*   Purpose         : Evaluation for if the Player is invisible
	*   Parameters      : float DeltaTime 
	*   Returns         : N/A
	*   Date altered    : 03/08/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void InvisibilityEvaluation( float DeltaTime );
	/*****************************************************************************
	*   Function        : void InvisibilityMaterial()
	*   Purpose         : Change Player material to turn invisible
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 03/08/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void InvisibilityMaterial();
	/*****************************************************************************
	*   Function        : void AmmoTick()
	*   Purpose         : Tick to handle ammunition and reserve
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 03/08/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void AmmoTick();
	/*****************************************************************************
	*   Function        : void AmmoEvaluation( int ammoCount, int reserveCount, int fullMag )
	*   Purpose         : Evaluate ammunition and reserve tank
	*   Parameters      : int ammoCount, int reserveCount, int fullMag 
	*   Returns         : N/A
	*   Date altered    : 03/08/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void AmmoEvaluation( int ammoCount, int reserveCount, int fullMag );
	/*****************************************************************************
	*   Function        : void HitmarkerFinished()
	*   Purpose         : Set hitmarker inactive
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 03/08/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void HitmarkerFinished();


public:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "MeleeTakedown")
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

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "MeleeTakedown" )
		bool takedownAvailable;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UChildActorComponent* primaryGun;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UChildActorComponent* pistol;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SFX" )
		USoundBase* callEnemy;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SFX" )
		USoundBase* footstepsSFX;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Weapons" )
		int m_currentlyEquipped; // Currently equipped weapon

	UPROPERTY( BlueprintCallable, BlueprintAssignable )
		FPlayerUI userInterfaceD;
public:
	/*****************************************************************************
	*   Function        : ACPP_PlayerManager()
	*   Purpose         : Constructor
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 02/05/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
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
	/*****************************************************************************
	 *   Function        : void SetCanTakedown( bool canTakedown );
	 *   Purpose         : Set bool for being able to stealth takedown AI
	 *   Parameters      : bool canTakedown
	 *   Returns         : N/A 
	 *   Date altered    : 30/05/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
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
	  *   Function        : void UserInterfaceDelegate()
	  *   Purpose         : Delegate for Players gameplay HUD
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		void UserInterfaceDelegate();
	/*****************************************************************************
	  *   Function        : void Invisibility()
	  *   Purpose         : Turn player invisibility on or off
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void Invisibility();
	/*****************************************************************************
	  *   Function        : bool GetInvisibilityStatus()
	  *   Purpose         : Get if invsibility is on
	  *   Parameters      : N/A
	  *   Returns         : m_invisibility
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	bool GetInvisibilityStatus();
	/*****************************************************************************
	  *   Function        : void InvisibilityFinished()
	  *   Purpose         : Actions when invisiblity has finished
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void InvisibilityFinished();
	/*****************************************************************************
	  *   Function        : void Respawn()
	  *   Purpose         : Respawn Player
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void Respawn();
	/*****************************************************************************
	  *   Function        : void IncreaseAmmoCount( int ammo )
	  *   Purpose         : Increase the ammunition of weapons when called
	  *   Parameters      : int ammo
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		void IncreaseAmmoCount( int ammo );
	/*************************************************************************************
	  *   Function        : void EquipGun( TArray<UChildActorComponent*> WeaponInventory )
	  *   Purpose         : Choose which gun is equipped by the player 
	  *   Parameters      : TArray<UChildActorComponent*> WeaponInventory
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : void ChangeWeapons( float inputAxis )
	  *************************************************************************************/
	void EquipGun( TArray<UChildActorComponent*> WeaponInventory );
	/*************************************************************************************
	  *   Function        : void ChangeWeapons( float inputAxis )
	  *   Purpose         : Change which weapons is chosen
	  *   Parameters      : float inputAxis
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : void EquipGun( TArray<UChildActorComponent*> WeaponInventory )
	  *************************************************************************************/
	void ChangeWeapons( float inputAxis );
	/*****************************************************************************
	  *   Function        : bool GetAssaultRifle()
	  *   Purpose         : Get assault rifle bool
	  *   Parameters      : N/A
	  *   Returns         : m_assaultRifle
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		bool GetAssaultRifle();
	/*****************************************************************************
	  *   Function        : bool GetPistol()
	  *   Purpose         : Get pistol bool
	  *   Parameters      : N/A
	  *   Returns         : m_pistol
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		bool GetPistol();
	/*****************************************************************************
	  *   Function        : bool GetThrowable()
	  *   Purpose         : Get throwable bool
	  *   Parameters      : N/A
	  *   Returns         : m_throwable
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		bool GetThrowable();
	/*****************************************************************************
	  *   Function        : bool GetHitmarkerActive()
	  *   Purpose         : Get if hitmarker is active or not
	  *   Parameters      : N/A
	  *   Returns         : m_hitmarkerActive
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		bool GetHitmarkerActive();
	/*****************************************************************************
	  *   Function        : void SetHitmarkerActive( bool hitmarkerActive )
	  *   Purpose         : Set if hitmarker is active or not
	  *   Parameters      : bool hitmarkerActive
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void SetHitmarkerActive( bool hitmarkerActive );
	/*****************************************************************************
	  *   Function        : void DistractEnemy()
	  *   Purpose         : Distract the enemy with a sound
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void DistractEnemy();
	/*****************************************************************************
	  *   Function        : void SetDeathHitmarkerActive( bool deathHitmarkerActive )
	  *   Purpose         : Set specific hitmarker for when Player kills AI
	  *   Parameters      : bool deathHitmarkerActive
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void SetDeathHitmarkerActive( bool deathHitmarkerActive );
	/*****************************************************************************
	  *   Function        : bool GetDeathHitmarkerActive()
	  *   Purpose         : Get hitmarker for when AI is shot dead
	  *   Parameters      : N/A
	  *   Returns         : m_deathHitmarkerActive
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		bool GetDeathHitmarkerActive();
	/*****************************************************************************
	  *   Function        : void EquipWeaponTick()
	  *   Purpose         : Equip weapon within Tick
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void EquipWeaponTick();
	/*****************************************************************************
	  *   Function        : void HitmarkerTick()
	  *   Purpose         : Set hitmarker in tick 
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void HitmarkerTick();
	/*****************************************************************************
	  *   Function        : float GetHealth()
	  *   Purpose         : Get Player health
	  *   Parameters      : N/A
	  *   Returns         : health
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		float GetHealth();
	/*****************************************************************************
	  *   Function        : void PlayerDead()
	  *   Purpose         : Actions when Player is dead
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void PlayerDead();
	/*****************************************************************************
	  *   Function        : void Reset()
	  *   Purpose         : Reset Player stats
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		void Reset();
public:

	/*****************************************************************************
	 *   Function        : bool GetTurnRight()
	 *   Purpose         : Get turn right bool
	 *   Parameters      : N/A
	 *   Returns         : m_turnRight
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : Used in animation blueprint
	 *   See also        : N/A
	*****************************************************************************/
	UFUNCTION( BlueprintCallable )
		bool GetTurnRight();
	/*****************************************************************************
	 *   Function        : bool GetTurnLeft()
	 *   Purpose         : Get turn left bool
	 *   Parameters      : N/A
	 *   Returns         : m_turnLeft
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : Used in animation blueprint
	 *   See also        : N/A
	*****************************************************************************/
	UFUNCTION( BlueprintCallable )
		bool GetTurnLeft();
public:
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
	 *   Function        : virtual void StartAim()
	 *   Purpose         : Start aiming in
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 01/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	 *****************************************************************************/
	void StartAim();
	/*****************************************************************************
	*   Function        : void StopAim()
	*   Purpose         : Stop aiming in
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 01/07/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void StopAim();
	/*****************************************************************************
	*   Function        : void StartCover( FHitResult hit )
	*   Purpose         : Start stealth cover
	*   Parameters      : FHitResult hit
	*   Returns         : N/A
	*   Date altered    : 01/07/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void StartCover( FHitResult hit );
	/*****************************************************************************
	*   Function        : void StopCover()
	*   Purpose         : Stop stealth cover
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 01/07/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void StopCover();
	/*****************************************************************************
	  *   Function        : void WallTrace()
	  *   Purpose         : Trace wall for cover
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void WallTrace();
	/*****************************************************************************
	  *   Function        : bool CoverTrace( float inputAxis )
	  *   Purpose         : Line trace for cover
	  *   Parameters      : float inputAxis
	  *   Returns         : bHit, false
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	bool CoverTrace( float inputAxis );
	/*****************************************************************************
	  *   Function        : bool GetIsCrouched()
	  *   Purpose         : Get bool for crouching
	  *   Parameters      : N/A
	  *   Returns         : return m_isCrouched;
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	bool GetIsCrouched();
	/*****************************************************************************
	  *   Function        : void StartShooting()
	  *   Purpose         : Character to shoot
	  *   Parameters      : N/A
	  *   Returns         : N/A
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	void StartShooting();
	/*****************************************************************************
	  *   Function        : bool RightCoverTrace()
	  *   Purpose         : Line trace for right of the wall
	  *   Parameters      : N/A
	  *   Returns         : rightHit
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : RightCoverTrace(), LeftCoverTrace(),
	  *****************************************************************************/
	bool RightCoverTrace();
	/*****************************************************************************
	  *   Function        : bool LeftCoverTrace()
	  *   Purpose         : Line trace for left of the wall
	  *   Parameters      : N/A
	  *   Returns         : leftHit
	  *   Date altered    : 01/07/2022
	  *   Contributors    : Jaber Ahmed
	  *   Notes           : N/A
	  *   See also        : N/A
	  *****************************************************************************/
	bool LeftCoverTrace();
	/*****************************************************************************
	 *   Function        : void ThrowObject()
	 *   Purpose         : Throw projectile
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : CPP_Throwable.h
	*****************************************************************************/
	void ThrowObject();
	/*****************************************************************************
	 *   Function        : void CreatePredictionSpline()
	 *   Purpose         : Create prediction spline for projectile
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void CreatePredictionSpline();
	/*****************************************************************************
	 *   Function        : void DestroyPredictionSpline()
	 *   Purpose         : Destroy prediction spline for projectile
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void DestroyPredictionSpline();
	/*****************************************************************************
	 *   Function        : void DestroyPredictionMeshes()
	 *   Purpose         : Destroy meshes for prediction spline
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void DestroyPredictionMeshes();
	/*****************************************************************************
	 *   Function        : void DrawPredictionSpline()
	 *   Purpose         : Draw prediction spline for projectiles
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 31/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	*****************************************************************************/
	void DrawPredictionSpline();
	/*****************************************************************************
 *   Function        : void Turn( float inputAxis )
 *   Purpose         : Mouse input turn
 *   Parameters      : float inputAxis
 *   Returns         : N/A
 *   Date altered    : 31/07/2022
 *   Contributors    : Jaber Ahmed
 *   Notes           : N/A
 *   See also        : N/A
*****************************************************************************/
	void Turn( float inputAxis );


	AActor* TakedownTrace();

};
