#ifndef MACRO_CMD_OP
#error  Define MACRO_CMD_OP before including this file.
#endif

//           enum name
MACRO_CMD_OP(Invalid    )
MACRO_CMD_OP(Queue      )
MACRO_CMD_OP(PlayAI     )
MACRO_CMD_OP(GameVictory)

// Unit related
MACRO_CMD_OP(Action     )
MACRO_CMD_OP(Move       )
MACRO_CMD_OP(BasicAttack)
MACRO_CMD_OP(Magic      )
MACRO_CMD_OP(Hit        )
MACRO_CMD_OP(Miss       )
MACRO_CMD_OP(Killed     )
MACRO_CMD_OP(EndTurn    )
MACRO_CMD_OP(Stay       )
MACRO_CMD_OP(Speak      )
MACRO_CMD_OP(GameEnd    )
MACRO_CMD_OP(RestoreHp  )
MACRO_CMD_OP(EndAction  )
MACRO_CMD_OP(GainExp    )

#undef MACRO_CMD_OP

