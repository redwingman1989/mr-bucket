

enum parserStates_ {
  PARSE_SYNC_1,
  PARSE_SYNC_2,
  PARSE_MSG_ID, 
  PARSE_DATA_LEN,
  PARSE_DATA,
  PARSE_CRC,
  NUM_PARSER_STATES
};
