// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h" 
#include "GameplayHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API AGameplayHUD : public AHUD
{
	GENERATED_BODY()
protected:
	// Centre
	UPROPERTY( EditDefaultsOnly )
		UTexture2D* CrosshairTexture;
public:
	// Primary draw call for HUD
	virtual void DrawHUD() override;
};
