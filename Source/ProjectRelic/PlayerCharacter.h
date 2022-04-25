// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterManager.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.generated.h"

/*************************************************************************************************
 * Type: Class
 *
 * Name: PlayerCharacter
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Child class for the player 
 *
 * Functions: APlayerCharacter(), ~APlayerCharacter(), 
 *   virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override;
 *
 * References:
 *
 * See Also:
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 15/03/2022    JA			 1.0         Prototype version
 * 25/04/2022	 JA			 1.1		 Adding invisiblity
 **************************************************************************************************/
UCLASS()
class PROJECTRELIC_API APlayerCharacter : public ACharacterManager
{
	GENERATED_BODY()
private:
	bool m_invisible;
	float m_invisibilityPercent;

	void Invisibility();

	void Tick(float DeltaTime) override;
	FTimerHandle m_invisibilityTimer;

	void InvisibilityTimer();

	UMaterialInterface* m_material;

	UMaterialInstanceDynamic* dynamicMaterial;
public:
	/********************************************************
	   *   Function        :
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	APlayerCharacter();


	~APlayerCharacter();

	/********************************************************
	   *   Function        :
	   *   Purpose         :
	   *   Parameters      :
	   *   Returns         :
	   *   Date altered    :
	   *   Contributors    : Jaber Ahmed
	   *   Notes           :
	   *   See also        :
	*********************************************************/
	void TakedownTrace();

	void Takedown();

	void PrepareTakedown();

	void SetInvisible( bool invisible );
	bool GetInvisible() const;

	virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override;

	virtual void BeginPlay() override;

	
};
