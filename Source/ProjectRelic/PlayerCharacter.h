// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterManager.h"

#include "PlayerCharacter.generated.h"

/**
 *
 */
UCLASS()
class PROJECTRELIC_API APlayerCharacter : public ACharacterManager
{
	GENERATED_BODY()

private:
	
	
public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override;

protected:

};
