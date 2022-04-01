// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API AEnemyCharacter : public ACharacterManager
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
		void OnPlayerCaught( APawn* pawn );
public:
	AEnemyCharacter();
	~AEnemyCharacter();
	void UpdateWalkSpeed( float chaseSpeed );
	UPROPERTY( EditAnywhere, Category = AI )
		class UBehaviorTree* behaviourTree;

	UPROPERTY( VisibleAnywhere, Category = AI )
		class UPawnSensingComponent* pawnSensingComp;

	void BeginPlay() override;

	FORCEINLINE AEnemyCharacter* GetEnemyCharacter( APawn* Pawn ) const;
protected:

};
