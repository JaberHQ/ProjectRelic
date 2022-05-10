// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterManager.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.generated.h"

/****************************************************************************************************************
 * Type: Class
 *
 * Name: PlayerCharacter (Prototype)
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Child class for the player 
 *
 * Functions: APlayerCharacter(), ~APlayerCharacter(), 
 *			  virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override,
 *			  virtual void BeginPlay() override, void TakedownTrace(), void Takedown(), void PrepareTakedown(),
 *			  void SetInvisible( bool invisible ), bool GetInvisible() const
 *
 * References: N/A
 *
 * See Also: CharacterManager.h
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 15/03/2022    JA			 v1.0        Prototype version
 * 11/04/2022	 JA			 v1.1		 Melee and cover
 * 25/04/2022	 JA			 v1.2		 Adding invisiblity
 * 02/05/2022	 JA			 v1.21		 Depreciated, going to be using CPP_PlayerCharacter
****************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API APlayerCharacter : public ACharacterManager
{
	GENERATED_BODY()

private:
	bool m_invisible; // If player is invisible
	float m_invisibilityPercent; // Invisibility bar percentage / duration
	float m_invisibilityFull; // Default invsibility bar percentage

	FTimerHandle m_invisibilityTimer; // Timer for invisibility duration
	UMaterialInterface* m_material; // Material 
	UMaterialInstanceDynamic* dynamicMaterial; // Dynamic material

private:
	/********************************************************
	   *   Function        : void Invisibility()
	   *   Purpose         : Set player invisible or visible
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void Invisibility();
	/********************************************************
	   *   Function        : void InvisibilityTimer()
	   *   Purpose         : Clear invisiblity timer
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	void InvisibilityTimer();
	/*************************************************************
	   *   Function        : void Tick( float DeltaTime ) override
	   *   Purpose         : Event tick
	   *   Parameters      : float DeltaTime
	   *   Returns         : N/A
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*************************************************************/
	void Tick( float DeltaTime ) override;

public:
	/********************************************************
	   *   Function        : APlayerCharacter()
	   *   Purpose         : Constructor
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	APlayerCharacter();
	/********************************************************
	   *   Function        : ~APlayerCharacter()
	   *   Purpose         : Deconstructor
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	~APlayerCharacter();
	/*************************************************************
	   *   Function        : void TakedownTrace()
	   *   Purpose         : Raycast line trace for melee takedown
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	**************************************************************/
	void TakedownTrace();
	/*************************************************************
	   *   Function        : void Takedown()
	   *   Purpose         : Takedown enemy 
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*************************************************************/
	void Takedown();
	/**********************************************************
	   *   Function        : void PrepareTakedown()
	   *   Purpose         : Get player (and enemy) in position
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	***********************************************************/
	void PrepareTakedown();
	/************************************************************
	   *   Function        : void SetInvisible( bool invisible )
	   *   Purpose         : Set invisible bool
	   *   Parameters      : bool invisible
	   *   Returns         : N/A
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	************************************************************/
	void SetInvisible( bool invisible );
	/********************************************************
	   *   Function        : bool GetInvisible() const
	   *   Purpose         : Get invisible
	   *   Parameters      : N/A
	   *   Returns         : return m_invisible
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	bool GetInvisible() const;
	/**********************************************************************************************************************
	   *   Function        : virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override
	   *   Purpose         : Inputs
	   *   Parameters      : class UInputComponent* playerInputComponent
	   *   Returns         : N/A
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	**********************************************************************************************************************/
	virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override;
	/**********************************************************
	   *   Function        : virtual void BeginPlay() override
	   *   Purpose         : Begin play event
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 25/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	***********************************************************/
	virtual void BeginPlay() override;
};
