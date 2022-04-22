// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyCharacter.h"
#include "Components/ProgressBar.h"
#include "WidgetDetectionMeter.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTRELIC_API UWidgetDetectionMeter : public UUserWidget
{
	GENERATED_BODY()
	
private:
	bool m_hasBeenSeen;
	float m_sightValuePercent;
	float m_detectionSpeed;
	
	UPROPERTY()
		UProgressBar* progressBar;
public:
	

	void Construct( bool isDesignTime, const TArray<AActor*>& caughtActors );

	FSightRegisteredD sightRegisteredDelegate;

	UFUNCTION()
		void RespondToDetection( bool hasBeenSeen, float detectionSpeed, const TArray<AActor*>& caughtActors );

};
