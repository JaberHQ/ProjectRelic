// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyPatrolPoint.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyController.generated.h"
/******************************************************************************************************************************
 * Type: Class
 *
 * Name: EnemyController
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Control the enemy patrol pawn and set its behaviour
 *
 * Functions: virtual void OnPossess( APawn* pawn ) override, AEnemyController(),
 *			  ~AEnemyController(), void SetPlayerCaught( APawn* pawn ), 
 *			  FORCEINLINE UBlackboardComponent* GetBlackboardComp() const,
 *			  FORCEINLINE TArray<AActor*> GetPatrolPoints() const,
 *		  	  void SetHasLineOfSight( bool hasLineOfSight ),
 *			  bool GetHasLineOfSight()
 *
 * References: Reubs via YouTube, https://www.youtube.com/watch?v=3Z1A825gTA8&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=4
 *				[Accessed: 20th March 2022 | Has been modified]
 *
 * See Also: EnemyCharacter, EnemyController, EnemyPatrolPoint, SelectEnemyPatrolPoint
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 31/03/2022    JA          1.0         Prototype, very basic pathing and vision
 ******************************************************************************************************************************/


UCLASS()
class PROJECTRELIC_API AEnemyController : public AAIController
{
	GENERATED_BODY()

private:
	UBehaviorTreeComponent* behaviourComp; // Behaviour Tree comp
	UBlackboardComponent* blackboardComp; // Blackboard Comp

	// BB keys
	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName locationToGoKey; // Patrol location

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName playerKey; // Target

	UPROPERTY( EditDefaultsOnly, Category = AI )
		FName hasLineOfSight; // Has line of sight bool

	

	TArray<AActor*> patrolPoints; // Patrol points array
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
	int32 currentPatrolPoint = 0;
	/********************************************************
	   *   Function        : AEnemyController()
	   *   Purpose         : Constructor
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	AEnemyController();
	/********************************************************
	   *   Function        : ~AEnemyController()
	   *   Purpose         : Deconstructor
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	~AEnemyController();
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
	/***********************************************************************************
	   *   Function        : FORCEINLINE UBlackboardComponent* GetBlackboardComp() const
	   *   Purpose         : Get blackboard component
	   *   Parameters      : N/A
	   *   Returns         : blackboardComp
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	************************************************************************************/
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const;
	/***********************************************************************************
	   *   Function        : FORCEINLINE TArray<AActor*> GetPatrolPoints() const
	   *   Purpose         : Get array of patrol points
	   *   Parameters      : N/A 
	   *   Returns         : patrolPoints
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	***********************************************************************************/
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const;

	//void UnPossess();


	

};
