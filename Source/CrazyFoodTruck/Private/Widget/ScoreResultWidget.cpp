// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/ScoreResultWidget.h"

#include "Components/TextBlock.h"

void UScoreResultWidget::SetScoreData(int32 InFinalScore, int32 InTimeScore, int32 InKillScore, EScoreGrade InGrade)
{
	if (FinalScoreText)
	{
		FinalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total score: %d"), InFinalScore)));
	}
	if (TimeScoreText)
	{
		TimeScoreText->SetText(FText::FromString(FString::Printf(TEXT("Time score: %d"), InTimeScore)));
	}
	if (KillScoreText)
	{
		KillScoreText->SetText(FText::FromString(FString::Printf(TEXT("Kill score: %d"), InKillScore)));
	}
	if (GradeText)
	{
		GradeText->SetText(FText::FromString(FString::Printf(TEXT("Grade: %s"), *GradeToString(InGrade))));
	}
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
