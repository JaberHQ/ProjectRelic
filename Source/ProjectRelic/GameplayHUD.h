// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h" 
#include "GameplayHUD.generated.h"

/***************************************************************************************
 * Type: Class
 *
 * Name: GameplayHUD
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Player's heads up display for the game
 *
 * Functions: virtual void DrawHUD() override
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 28/03/2022    JA          v1.0        Created a HUD for the crosshair
 ***************************************************************************************/
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
