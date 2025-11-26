// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/ScoreResultWidget.h"

#include "Components/TextBlock.h"

void UScoreResultWidget::SetScoreData(int32 Time, int32 KillCount, int LifeRemaining, EScoreGrade InGrade)
{
	OnScoreShow.Broadcast(Time, KillCount, LifeRemaining, InGrade);
}

void UScoreResultWidget::EndScoreDisplay()
{
	OnScoreEnded.Broadcast();
}

FString UScoreResultWidget::GradeToString(EScoreGrade Grade) const
{
	switch (Grade)
	{
	case EScoreGrade::S: return TEXT("S");
	case EScoreGrade::A: return TEXT("A");
	case EScoreGrade::B: return TEXT("B");
	case EScoreGrade::C: return TEXT("C");
	case EScoreGrade::D: return TEXT("D");
	case EScoreGrade::E: return TEXT("E");
	case EScoreGrade::F: default: return TEXT("F");
	}
}
