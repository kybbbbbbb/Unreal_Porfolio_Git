#include "Characters/CAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "CMonsters_AI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "../Components/CAIBehaviorComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "CPlayer.h"

ACAIController::ACAIController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackBoard");
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	DamageConfigs = CreateDefaultSubobject<UAISenseConfig_Damage>("DamageSense");
	DamageConfigs->SetMaxAge(30.0f);

	Perception->ConfigureSense(*Sight);
	Perception->ConfigureSense(*DamageConfigs);
}



void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	SetSightConfigByIndex(0);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdate);
}


void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Monster = Cast<ACMonsters_AI>(InPawn);
	if (Monster != nullptr)
	{
		SetGenericTeamId(Monster->GetTeamID());
	}

	if (Monster->GetBeheviorTree() == nullptr)
		return;

	UBlackboardComponent* blackboard = Blackboard.Get();

	UseBlackboard(Monster->GetBeheviorTree()->BlackboardAsset, blackboard);
	RunBehaviorTree(Monster->GetBeheviorTree());

	AIBehavior = Monster->GetAIBehevior();
	AIBehavior->SetBlackboard(blackboard);
	

}

void ACAIController::OnUnPossess()
{

	Super::OnUnPossess();
}



void ACAIController::OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	if (AIBehavior->IsDeadMode())
	{
		Dead();
		return;
	}



	TArray<AActor*> actors;
	//데미지 감지시
	Perception->GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), actors);
	if (actors.Num() > 0)
	{
		if (AIBehavior->IsDeadMode())
		{
			Dead();
			return;
		}
		for (int32 i = 0; i < actors.Num(); i++)
		{
			if (actors[i]->IsA(ACPlayer::StaticClass()))
			{
				ACPlayer* player = Cast<ACPlayer>(actors[i]);
				if (CurrentPlayerSave == nullptr)
				{
					bPlayerAttackable = player->RequestAttackToken();
					CurrentPlayerSave = player;
					Blackboard->SetValueAsObject("Target", player);
					return;
				}

				if (CurrentPlayerSave != nullptr)
				{
					if (bPlayerAttackable == false)
					{
						//플레이터 상태 다시 체크
						bPlayerAttackable = player->RequestAttackToken();
						return;
					}

				}
			}
		}
	}





	//시야에서 가장 가까운 적을 감지
	Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), actors);

	AActor* NearestTarget = nullptr;
	float MinDistance = TNumericLimits<float>::Max();  

	for (AActor* Actor : actors)
	{

		float Distance = FVector::Dist(Actor->GetActorLocation(), GetPawn()->GetActorLocation());

		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestTarget = Actor;
		}
		
	}

	//감지된 타겟이 없을때
	if (NearestTarget == nullptr)
	{
		//감지타겟이 없고 플레이어도 저장된게 없다면
		if (CurrentPlayerSave == nullptr)
		{
			if (bPlayerAttackable == true)
			{
				bPlayerAttackable = false;
			}

			Blackboard->SetValueAsObject("Target", nullptr);
			return;
		}

		//감지타겟이 없지만 플레이어가 저장된게 있다면
		if (CurrentPlayerSave != nullptr)
		{
			if (bPlayerAttackable == true)
			{
				ACPlayer* player = Cast<ACPlayer>(CurrentPlayerSave);
				player->ReleaseAttackToken();
				bPlayerAttackable = false;
				CurrentPlayerSave = nullptr;
				Blackboard->SetValueAsObject("Target", nullptr);
				return;
			}

			if (bPlayerAttackable == false)
			{
				Blackboard->SetValueAsObject("Target", nullptr);
				CurrentPlayerSave = nullptr;
				return;
			}
		}

	}


	// 가장 가까운 타겟을 설정
	if (NearestTarget)
	{



		//현재 저장된 플레이어가 없을때 로직
		if (CurrentPlayerSave == nullptr)
		{
			//저장된 플레이어가 없고 들어온 타겟이 플레이어면?
			if (NearestTarget->IsA(ACPlayer::StaticClass()))
			{
				ACPlayer* player = Cast<ACPlayer>(NearestTarget);
				bPlayerAttackable = player->RequestAttackToken();
				CurrentPlayerSave = player;

				Blackboard->SetValueAsObject("Target", player);
				return;

			}
			else //타겟이 플레이어가 아니면
			{
				Blackboard->SetValueAsObject("Target", NearestTarget);
				return;
			}
		}




		//만약 저장된 플레이어가 존재한다면
		if (CurrentPlayerSave != nullptr)
		{
			
			//저장된 플레이어가 존재하고 들어온 타겟이 동일한 플레이어면?
			if (NearestTarget == CurrentPlayerSave)
			{
				
				//현재 플레이어 공격 불가능 상태라면 (어라운드)
				if (bPlayerAttackable == false)
				{
					//플레이터 상태 다시 체크
					ACPlayer* player = Cast<ACPlayer>(CurrentPlayerSave);
					bPlayerAttackable = player->RequestAttackToken();
					return;
				}

				//현재 플레이어 공격 가능상태이면
				if(bPlayerAttackable == true)
					return;
			}
			else //저장된 플레이어가 존재하고 들어온 타겟이 플레이어가 아니면?
			{
				if (bPlayerAttackable == false)
				{
					Blackboard->SetValueAsObject("Target", NearestTarget);
					CurrentPlayerSave = nullptr;
					return;
				}

				if (bPlayerAttackable == true)
				{
					ACPlayer* player = Cast<ACPlayer>(CurrentPlayerSave);
					Blackboard->SetValueAsObject("Target", NearestTarget);
					player->ReleaseAttackToken();
					CurrentPlayerSave = nullptr;
					bPlayerAttackable = false;
					return;
				}
			}
		}
	}



}



void ACAIController::SetSightConfigByIndex(int32 Index)
{
	if (SightConfigs.IsValidIndex(Index))
	{
		const FSightConfig& Config = SightConfigs[Index];

		Sight->SightRadius = Config.SightRadius;
		Sight->LoseSightRadius = Config.LoseSightRadius;
		Sight->PeripheralVisionAngleDegrees = Config.PeripheralVisionAngleDegrees;

		Sight->DetectionByAffiliation.bDetectEnemies = Config.bDetectEnemies;
		Sight->DetectionByAffiliation.bDetectNeutrals = Config.bDetectNeutrals;
		Sight->DetectionByAffiliation.bDetectFriendlies = Config.bDetectFriendlies;

		Perception->ConfigureSense(*Sight);
	}
}

void ACAIController::Dead()
{

	if (CurrentPlayerSave == nullptr)
	{
		Blackboard->SetValueAsObject("Target", nullptr);
		return;
	}

	if (CurrentPlayerSave != nullptr)
	{
		if (bPlayerAttackable == true)
		{
			ACPlayer* player = Cast<ACPlayer>(CurrentPlayerSave);
			player->ReleaseAttackToken();
			bPlayerAttackable = false;
			CurrentPlayerSave = nullptr;
			Blackboard->SetValueAsObject("Target", nullptr);
			return;
		}

		if (bPlayerAttackable == false)
		{
			Blackboard->SetValueAsObject("Target", nullptr);
			CurrentPlayerSave = nullptr;
			return;
		}
	}
	return;

}
