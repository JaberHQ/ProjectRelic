// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyCharacter.h"
#include "Components/ProgressBar.h"
#include "WidgetDetectionMeter.generated.h"

/***************************************************************************************
 * Type: Class
 *
 * Name: WidgetDetectionMeter 
 *
 * Author: Jaber Ahmed
 *
 * Purpose: User widget for sight detection (Prototype)
 *
 * Functions: void Construct( APawn* pawn )
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 18/04/2022    JA          v1.0		 Created BP widget
 *****************************************************************************************/

UCLASS()
class PROJECTRELIC_API UWidgetDetectionMeter : public UUserWidget
{
	GENERATED_BODY()

private:
	UProgressBar* m_progressBar; // Progress bar
	APawn* m_pawn; // Pawn reference

public:
	/*****************************************************************************
	*  Function         : void Construct( APawn* pawn )
	*   Purpose         : Construct event
	*   Parameters      : APawn* pawn
	*   Returns         : N/A
	*   Date altered    : 18/04/2022 
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	void Construct( APawn* pawn );
};
