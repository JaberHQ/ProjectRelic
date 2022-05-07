// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_CharacterManager.h"
#include "CPP_PlayerManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API ACPP_PlayerManager : public ACPP_CharacterManager
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	
};
