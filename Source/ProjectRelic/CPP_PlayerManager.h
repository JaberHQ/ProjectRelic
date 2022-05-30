// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_CharacterManager.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
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
**************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_PlayerManager : public ACPP_CharacterManager
{
	GENERATED_BODY()
private:
	bool m_canTakedown;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MeleeTakedown")
		float m_takedownTraceDistance;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animations" )
		UAnimMontage* animTakedown;

	UPROPERTY( EditAnywhere, Category = "Health" )
		float m_health;

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

	void SetCanTakedown( bool canTakedown );

	bool GetCanTakedown();
	
	void Takedown();

	void TraceForward_Implementation();

	void AnimationExecuted();

	virtual void TakeAttack() override;
};
