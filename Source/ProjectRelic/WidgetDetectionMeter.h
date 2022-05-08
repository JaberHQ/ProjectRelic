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
 * Purpose: User widget for sight detection
 *
 * Functions: void Construct( APawn* pawn )
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * ----------    JA          -------     ----------------------------------------------
 *****************************************************************************************/

UCLASS()
class PROJECTRELIC_API UWidgetDetectionMeter : public UUserWidget
{
	GENERATED_BODY()

private:
	UProgressBar* m_progressBar; // Progress bar
	APawn* m_pawn; // Pawn

public:
	void Construct( APawn* pawn );
};
