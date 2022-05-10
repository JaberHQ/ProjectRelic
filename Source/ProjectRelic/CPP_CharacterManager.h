// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_CharacterManager.generated.h"
/**************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_CharacterManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Version 2 (and above) of the CharacterManager
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
 **************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_CharacterManager : public ACharacter
{
	GENERATED_BODY()

private:
	bool m_isCrouched; // If Player is crouched

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USpringArmComponent* springArmComp; // Spring Arm Component to follow the camera behind the player

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UCameraComponent* cameraComp; // Player follow camera
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

	
};
