// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "CPP_AIController.generated.h"
/**************************************************************************************
 * Type: Class
 *
 * Name: CPP_AIController
 *
 * Author: Jaber Ahmed
 *
 * Purpose: AI Controller to control Enemy Behaviour
 *
 * Functions: ACPP_AIController(), UBlackboardComponent* GetBlackboardComp() const
 *
 * References: N/A
 *
 * See Also: N/A
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 02/05/2022   JA           v2.0        Create a better EnemyController
 **************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_AIController : public AAIController
{
	GENERATED_BODY()

private:
	UBehaviorTreeComponent* behaviourComp; // Behaviour Tree comp
	UBlackboardComponent* blackboardComp; // Blackboard Comp

	// BB keys
	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName m_patrolLocation; // Patrol location

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName m_enemyActor; // Target

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName m_hasLineOfSight; // Has line of sight bool

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName m_patrolPathVector; // Patrol path vector

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName m_patrolPathIndex; // Patrol path index

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName m_lastKnownLocation; // Patrol path index
	
	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName m_invesigate; // Patrol path index
private:
	/**********************************************************************
	   *   Function        : virtual void OnPossess( APawn* pawn ) override
	   *   Purpose         : Possess pawn and control behaviour
	   *   Parameters      : APawn* pawn
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	***********************************************************************/
	virtual void OnPossess( APawn* pawn ) override;

public:
	/*****************************************************************************
	*   Function        : ACPP_AIController()
	*   Purpose         : Constructor
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 02/05/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	ACPP_AIController();
	/**********************************************************************************
	*   Function        : FORCEINLINE UBlackboardComponent* GetBlackboardComp() const
	*   Purpose         : Get Blackboard component
	*   Parameters      : N/A
	*   Returns         : blackboardComp
	*   Date altered    : 02/05/2022 
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	**********************************************************************************/
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const;
	/***********************************************************************************
	   *   Function        : void SetPlayerCaught( const TArray<AActor*>& caughtActors )
	   *   Purpose         : Set the target to the player when spotted
	   *   Parameters      : const TArray<AActor*>& caughtActors
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	***********************************************************************************/
	void SetPlayerCaught( const TArray<AActor*>& caughtActors );
	/*****************************************************************************
		*   Function        : void SetHasLineOfSight( bool hasLineOfSight )
		*   Purpose         : Set BB key boolean
		*   Parameters      : bool boolean
		*   Returns         : N/A
		*   Date altered    : 09/04/2022
		*   Contributors    : Jaber Ahmed
		*   Notes           : N/A
		*   See also        : N/A
	*****************************************************************************/
	void SetHasLineOfSight( bool boolean );
	/*****************************************************************************
		*   Function        : bool GetHasLineOfSight()
		*   Purpose         : Get BB key hasLineOfSight
		*   Parameters      : N/A
		*   Returns         : hasLineOfSight
		*   Date altered    : 09/04/2022
		*   Contributors    : Jaber Ahmed
		*   Notes           : N/A
		*   See also        : N/A
	*****************************************************************************/
	bool GetHasLineOfSight();

	void SetLastKnownLocation( FVector vector );

	void SetInvestigate( bool boolean );

	FVector GetLastKnownLocation();
};
