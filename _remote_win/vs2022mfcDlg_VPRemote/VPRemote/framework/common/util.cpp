#include "pch.h"

#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



using namespace UTL;


int UTL::constrain(int amt, int low, int high)
{
  return ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)));
}

bool UTL::between(int amt, int low, int high)
{
  bool ret = true;
  if (amt < low) {
    ret = false;
  }
  else if (amt > high) {
    ret = false;
  }
  return ret;
}


int UTL::DwToInt(uint8_t* bytes)
{
  int a = bytes[0] & 0xFF;
  a |= ((bytes[1] << 8) & 0xFF00);
  a |= ((bytes[2] << 16) & 0xFF0000);
  a |= ((bytes[3] << 24) & 0xFF000000);

  return a;
}

uint32_t UTL::DwToUint(uint8_t* bytes)
{
  uint32_t a = bytes[0] & 0xFF;
  a |= ((bytes[1] << 8) & 0xFF00);
  a |= ((bytes[2] << 16) & 0xFF0000);
  a |= ((bytes[3] << 24) & 0xFF000000);

  return a;
}


void file::CurrDir(char* p_buff, int len)
{
  char* ret = _getcwd(p_buff, len);
}

int file::MakeDir(const char* dir_name)
{
  return _mkdir(dir_name);
}

int file::DeleteDir(const char* dir_name)
{
  return _rmdir(dir_name);
}

bool file::isFile(_finddata_t fd)
{
  if (fd.attrib & _A_SUBDIR)
    return false;
  return true;
}

bool file::isDir(_finddata_t fd)
{
  if (fd.attrib & _A_SUBDIR)
    return true;
  return false;
}

bool file::isDirExists(const char* dir)
{
  return (_access_s(dir, 0) == 0 ? true : false);
}

bool file::isFileExists(const char* file)
{
  return (_access_s(file, 0) == 0 ? true : false);
}

int file::SearchDir(const char* dir_str)
{
  std::string dir_name = dir_str;
  std::string path = dir_name + "\\*.*";
  int ret = 0;
  //path.append("\\*.*");
  struct _finddata_t fd;//���丮 �� ���� �� ���� ���� ���� ��ü
  intptr_t handle;
  list<_finddata_t> fd_list;//���丮, ���� ���� ��ü ����Ʈ

  if ((handle = _findfirst(path.c_str(), &fd)) == -1L) //fd ����ü �ʱ�ȭ.
  {
    //�����̳� ���丮�� ���� ���.
    TRACE("No file in directory!");
    goto returnOut;
  }

  do //���� Ž�� �ݺ� ����
  {
    bool is_file = isFile(fd);//���� ��ü ���� Ȯ��(���� or ���丮)
    if (is_file == false && fd.name[0] != '.') {
      //���丮�� ���� ����        
      ret += SearchDir((dir_name + "\\" + fd.name).c_str());//����� ȣ��
    }
    else if (is_file == true && fd.name[0] != '.') {
      //������ ���� ����       
      fd_list.push_back(fd);
    }

  } while (_findnext(handle, &fd) == 0);
  ret += (int)fd_list.size();
returnOut:
  _findclose(handle);
  return ret;
}

int file::GetFileCount(const char* dir_str, bool inc_dir)
{
  std::string dir_name = dir_str;
  std::string path = dir_name + "\\*.*";
  int ret = 0;
  struct _finddata_t fd;//���丮 �� ���� �� ���� ���� ���� ��ü
  intptr_t handle;
  if ((handle = _findfirst(path.c_str(), &fd)) == -1L) {
    //�����̳� ���丮�� ���� ���.
    TRACE("No file in directory!");
    goto returnOut;
  }

  do //���� Ž�� �ݺ� ����
  {
    bool is_file = isFile(fd);//���� ��ü ���� Ȯ��(���� or ���丮)
    if (is_file == false && fd.name[0] != '.') {
      //���丮�� ���� ����        
      if (inc_dir) {
        ret += GetFileCount((dir_name + "\\" + fd.name).c_str(), inc_dir); //����� ȣ��
      }
    }
    else if (is_file == true && fd.name[0] != '.') {
      //������ ���� ����  
      ret++;
    }

  } while (_findnext(handle, &fd) == 0);
returnOut:
  _findclose(handle);

  return ret;
}

bool file::isLeafDir(const char* dir_str)
{
  std::string dir_name = dir_str;
  std::string path = dir_name + "\\*.*";
  struct _finddata_t fd;//���丮 �� ���� �� ���� ���� ���� ��ü
  intptr_t handle;
  bool ret = false;

  //fd ����ü �ʱ�ȭ.
  if ((handle = _findfirst(path.c_str(), &fd)) == -1L) {
    //�����̳� ���丮�� ���� ���.
    TRACE("No file in directory!");
    ret = true;
    goto returnOut;
  }

  do //���� Ž�� �ݺ� ����
  {
    bool is_dir = isDir(fd);
    if (is_dir && fd.name[0] != '.') {
      goto returnOut;
    }
  } while (_findnext(handle, &fd) == 0);
  ret = true;
returnOut:
  _findclose(handle);
  return ret;
}

int file::DeleteFilesInDir(const char* dir_str, bool inc_dir)
{
  std::string dir_name = dir_str;
  std::string path = dir_name + "\\*.*";
  int ret = 0;
  struct _finddata_t fd;//���丮 �� ���� �� ���� ���� ���� ��ü
  intptr_t handle;
  if ((handle = _findfirst(path.c_str(), &fd)) == -1L) {
    //�����̳� ���丮�� ���� ���.
    TRACE("No file in directory!");
    goto returnOut;
  }

  do //���� Ž�� �ݺ� ����
  {
    bool is_file = isFile(fd);//���� ��ü ���� Ȯ��(���� or ���丮)
    if (is_file == false && fd.name[0] != '.') {
      //���丮�� ���� ����        
      if (inc_dir) {
        ret += DeleteFilesInDir((dir_name + "\\" + fd.name).c_str(), inc_dir); //����� ȣ��
      }
    }
    else if (is_file == true && fd.name[0] != '.') {
      //������ ���� ����  
      remove((dir_name + "\\" + fd.name).c_str());
      ret++;
    }

  } while (_findnext(handle, &fd) == 0);
returnOut:
  _findclose(handle);

  return ret;
}

errno_t file::CopyFilesInDir_binary(const char* dst_dir, const char* src_dir, bool over_write, bool inc_dir)
{
  std::string _src_path = src_dir;
  _src_path += "\\*.*";
  struct _finddata_t fd;
  intptr_t handle;
  FILE* src_file = {}, * dst_file = {};
  errno_t err = FILE_ERR_SUCCESS;

  std::string _dst_path = dst_dir;

  if ((handle = _findfirst(_src_path.c_str(), &fd)) == -1L) {
    //�ҽ� ���丮�� �����̳� ���丮�� ���� ���.
    TRACE("No file in directory!");
    err = FILE_ERR_NO_FILE_IN_DIR;
    goto returnOut;
  }

  _src_path = src_dir;
  do //���� Ž�� �ݺ� ����
  {
    bool is_file = isFile(fd);//���� ��ü ���� Ȯ��(���� or ���丮)

    if (fd.name[0] == '.')
      continue;

    if (is_file) {
      // ���� ī�� 
      if (FILE_ERR_SUCCESS == (fopen_s(&src_file, (_src_path + "\\" + fd.name).c_str(), "rb"))) {
        // ������ ������ ��ġ�� �̵��Ѵ�. 
        fseek(src_file, 0, SEEK_END);
        // ������ ��ġ���� ��ġ ���� �� ������ ũ�⸦ ����Ѵ�.
        long pos = ftell(src_file);
        // ������ ó�� ��ġ�� �����Ѵ�.
        char* buff = new char[size_t(pos) + 1];
        fseek(src_file, 0, SEEK_SET);
        // ������ ũ�⸸ŭ �����͸� �д´�.
        fread(buff, pos, 1, src_file);
        fclose(src_file);

        if (isDirExists(_dst_path.c_str()) == false) {
          MakeDir((_dst_path /*+ "\\" + fd.name*/).c_str());
        }

        // ���� ������ ������
        if (isFileExists((_dst_path + "\\" + fd.name).c_str())) {
          if (over_write) {
            if (FILE_ERR_SUCCESS == (fopen_s(&dst_file, (_dst_path + "\\" + fd.name).c_str(), "wb"))) {
              // ��� ���� �����
              fwrite((const void*)buff, sizeof(char), size_t(pos) + 1, dst_file);
              fclose(dst_file);
            }
          }
        }
        else {
          if (FILE_ERR_SUCCESS == (fopen_s(&dst_file, (_dst_path + "\\" + fd.name).c_str(), "wb"))) {
            // ��� ���� �����
            //fprintf(dst_file, buff);
            fwrite((const void*)buff, sizeof(char), size_t(pos) + 1, dst_file);
            fclose(dst_file);
          }
        }

        delete[] buff;
        buff = nullptr;
      }
    }
    else {
      if (inc_dir) {
        if (isDirExists(_dst_path.c_str()) == false) {
          MakeDir((_dst_path /*+ "\\" + fd.name*/).c_str());
        }

        CopyFilesInDir_binary((_dst_path + "\\" + fd.name).c_str()
          , (_src_path + "\\" + fd.name).c_str()
          , over_write
          , inc_dir);
      }
    }

  } while (_findnext(handle, &fd) == 0);

returnOut:
  _findclose(handle);

  return err;
}


errno_t file::FileCopy(const char* psour, const char* pdest) {

  FILE* fsour, * fdest;
  char   buff[1024];
  size_t     n_size;

  if (!strcmp(psour, pdest))
    return FILE_ERR_NO_DIFFERENCE; // ������ �纻 ������ �����ϸ� ����

  if (ERROR_SUCCESS != (fopen_s(&fsour, psour, "rb")))
    return FILE_ERR_READ_FILE; // ���� ���� ����

  if (ERROR_SUCCESS != fopen_s(&fdest, pdest, "wb"))
  {
    fclose(fsour);
    return FILE_ERR_MAKE_COPYFILE; // ���� ���� ����
  }


  // ��� ���� �����

  if (fsour != nullptr)
  {
    while (0 < (n_size = fread(buff, 1, sizeof(buff), fsour))) {
      if (0 == fwrite(buff, 1, n_size, fdest)) {
        fclose(fsour);
        fclose(fdest);
        _unlink(pdest); // ������ ���� ����� ����
        return FILE_ERR_WRITE_COPYFILE;
      }
    }
    fclose(fsour);
    fclose(fdest);
  }



  return FILE_ERR_SUCCESS;
}


uint8_t trans::SplitArgs(char* arg_str, char** argv, char* delim_chars, int max)
{
  uint8_t argc = 0;
  char* tok = nullptr;
  char* next_ptr = nullptr;

  for (tok = strtok_s(arg_str, delim_chars, &next_ptr); tok; tok = strtok_s(NULL, delim_chars, &next_ptr))
  {
    argv[argc++] = tok;
    if (argc >= max)
      return argc - 1;
  }
  return argc;
}

void trans::NowStr(char* p_str, int max_len)
{
  SYSTEMTIME st;
  GetLocalTime(&st);

  sprintf_s(p_str, max_len, "[%04d/%02d/%02d] %02d:%02d:%02d'%02d \n"
    , st.wYear
    , st.wMonth
    , st.wDay
    , st.wHour
    , st.wMinute
    , st.wSecond
    , st.wMilliseconds);

}


void trans::DateFormStr(char* p_str, int length, DataTimeFormat_e type)
{

  SYSTEMTIME st;
  GetLocalTime(&st);


  switch (type)
  {
  case DataTimeFormat_e::YYYYMMDD_HHMMSS_UU:
  {
    sprintf_s(p_str, length, "%04d-%02d-%02d %02d:%02d:%02d'%03d "
      , st.wYear
      , st.wMonth
      , st.wDay
      , st.wHour
      , st.wMinute
      , st.wSecond
      , st.wMilliseconds
    );
  }

  break;
  case DataTimeFormat_e::YYYYMMDD_HHMMSS_square_brackets:
  {
    sprintf_s(p_str, length, "[%04d-%02d-%02d] [%02d:%02d:%02d] "
      , st.wYear
      , st.wMonth
      , st.wDay
      , st.wHour
      , st.wMinute
      , st.wSecond
    );
  }
  break;
  case DataTimeFormat_e::YYYYMMDD_HHMMSS_space:
  default:
  {
    sprintf_s(p_str, length, "%04d-%02d-%02d %02d:%02d:%02d "
      , st.wYear
      , st.wMonth
      , st.wDay
      , st.wHour
      , st.wMinute
      , st.wSecond
    );
  }
  break;
  }


#if 0
  time_t timetClock;
  struct tm tmNewTime;

  // Get time in seconds 
  time(&timetClock);
  // Convert time to struct  tm form 
  localtime_s(&tmNewTime, &timetClock);

  switch (type)
  {
  case DataTimeFormat_e::YYYYMMDD_HHMMSS_space:
  {
    sprintf_s(p_str, length, "%04d-%02d-%02d %02d:%02d:%02d "
      , tmNewTime.tm_year + 1900
      , tmNewTime.tm_mon + 1
      , tmNewTime.tm_mday
      , tmNewTime.tm_hour
      , tmNewTime.tm_min
      , tmNewTime.tm_sec
    );
  }
  break;
  case DataTimeFormat_e::YYYYMMDD_HHMMSS_square_brackets:
  default:
  {
    sprintf_s(p_str, length, "[%04d-%02d-%02d] [%02d:%02d:%02d] "
      , tmNewTime.tm_year + 1900
      , tmNewTime.tm_mon + 1
      , tmNewTime.tm_mday
      , tmNewTime.tm_hour
      , tmNewTime.tm_min
      , tmNewTime.tm_sec
    );
  }
  break;
  }
#endif

}

void trans::Left(const char* pdata, char* pdest, int size, int count)
{
  count = UTL::constrain(count, 0, (int)strlen(pdata));
  int i = 0;
  for (; i < count; i++) {
    pdest[i] = pdata[i];
  }
  pdest[i] = '\0';
}


char* trans::FindChar(const char* p_str, int size, char* find_char, bool is_back/* = true*/)
{
  int i = 0;
  if (is_back)
  {

  }
  else
  {

  }

  return 0;
}


void trans::Replace(char* src, char find, bool upcase, char replace)
{
  size_t size = strlen(src);
  size_t pos = 0;
  size_t idx = 0;
  char tmp = 0x0;
  while (size) {
    tmp = src[pos];
    src[pos] = 0x00;
    if (tmp == find) {
      if (replace != ' ') {
        src[idx++] = replace;
      }
    }
    else {
      if (upcase)
        src[idx++] = toupper(tmp);
      else
        src[idx++] = tmp;
    }
    pos++;
    size--;
  }
}


#ifdef _AFX

CString trans::CharToCString(char* str)
{
  CString cStr;
#if defined(UNICODE) || defined(_UNICODE)
  int nLen = (int)strlen(str) + 1;
  TCHAR* tszTemp = nullptr;
  tszTemp = new TCHAR[nLen];
  memset(tszTemp, 0x00, nLen * sizeof(TCHAR));
  MultiByteToWideChar(
    CP_ACP,
    0,
    str,
    -1,
    tszTemp,
    nLen
  );

  cStr.Format(_T("%s"), tszTemp);
  if (tszTemp)
  {
    delete[] tszTemp;
    tszTemp = NULL;
  }
#else
  cStr.Format("%s", str);
#endif
  return cStr;
}

CString trans::TcharToCString(TCHAR* str)
{
  CString cStr;
  cStr.Format(_T("%s"), str);
  return cStr;
}

#endif




int trans::CharToLPTSTR(LPSTR str, LPTSTR ret_str, int length)
{
  if (strlen(str) == 0)
    return 0;
  if (_tcslen(ret_str) != 0)
    memset(ret_str, 0x00, length);   /* _tcslen(ret_str) * sizeof(TCHAR));*/
  int ret = 0;

#if defined(UNICODE) || defined(_UNICODE)
  //int nLen = (int)_tcslen(ret_str) * (int)sizeof(TCHAR);
  ret = MultiByteToWideChar(
    CP_ACP,
    MB_PRECOMPOSED,
    str,
    (int)strlen(str),
    ret_str,
    length
  );
#else
  int nLen = strlen(str) + 1;
  _tcscpy(ret_str, str);
  ret = nLen;
#endif
  return ret;
}

int trans::TcharToLPSTR(LPTSTR str, LPSTR ret_str, int length)
{
  int ret = 0;
  if (_tcslen(str) == 0)
    return 0;
  if (length != 0)
    memset(ret_str, 0x00, length * sizeof(CHAR));

#if defined(UNICODE) || defined(_UNICODE)
  ret = WideCharToMultiByte(
    CP_ACP,
    0,
    str,
    -1,
    ret_str,
    length + 1,
    NULL,
    NULL
  );
#else
  strcpy(ret_str, str);

#endif
  return ret;
}

bool UTL::IsEqualStr(char* str_rf, char* str_cp, bool is_ignore)
{
  bool ret = true;
  char tmp_1, tmp_2 = 0x0;
  size_t size1 = strlen(str_rf);
  size_t size2 = strlen(str_cp);
  size_t idx = 0;
  if (size1 == size2) {
    while (size1) {
      tmp_1 = str_rf[idx];
      tmp_2 = str_rf[idx++];
      if (is_ignore) {
        if (toupper(tmp_1) != toupper(tmp_2)) {
          ret = false;
          break;
        }
      }
      else {
        if (tmp_1 != tmp_2) {
          ret = false;
          break;
        }
      }
      size1--;
    }
  }
  else
    ret = false;

  /*
  if (strcmp(str1, str2) == 0) {
    ret = true;
  }*/

  return ret;
}




int parser::json::LoadData()
{
  FILE* p_file = nullptr;
  /* Open for read (will fail if file "data" does not exist) */
  fopen_s(&p_file, m_dirFile, "rb");
  if (p_file == nullptr) {
    TRACE("The file 'data' was not opened! \n\r");
    return -1;
  }
  else
    TRACE("The file 'data' was opened \n\r");

  // ���� ũ�� ���ϱ�
  fseek(p_file, 0, SEEK_END);
  size_t size = ftell(p_file);
  fseek(p_file, 0, SEEK_SET);

  // ���� ũ�� + NULL ������ŭ �޸𸮸� �Ҵ��ϰ� 0���� �ʱ�ȭ

  std::vector<char> elems(size+1);
 
  if (elems.size() > 1)
  {
    if (fread(elems.data(), size, 1, p_file) > 0) {
      //�Ľ��Ͽ� 
      parsing(elems.data(), (uint32_t)size);
    }
    else {
      size = 0;
    }
  }

  /*
   
  char* file_binary = nullptr;

  file_binary = new char[size + 1] { 0 };
  if (file_binary != nullptr)
  {
    // ���� ���� �б�
    if (fread(file_binary, size, 1, p_file) > 0) {
      //�Ľ��Ͽ� 
      parsing(file_binary, (uint32_t)size);
    }
    else {
      size = 0;
    }
  }
  delete[] file_binary;
  */
  return (uint32_t)m_incNo; //(uint32_t)size;
}

void parser::json::parsing(char* p_data, uint32_t size)
{

#if 1
  enum class state_t {
    section,
    key,
    value,
    partition,
  };

  uint32_t curr = 0;
  //uint32_t addr = 0;
  bool is_pass_cnt = false;
  char data;
  state_t state = state_t::section;
  if (p_data[curr] != '{' && p_data[curr] != '[')
    return;

  m_vData.push_back(json_t{});
  curr++;
  // ���� ũ�⸸ŭ �ݺ�
  while (curr < size)
  {
    data = p_data[curr];
    switch (state)
    {
    case state_t::section:
    {
      switch (data)
      {
      case '\"':
      {
        state = state_t::key;
      }
      break;
      case '/':
      {
        if (p_data[curr + 1] == '/') // �ּ� ó��
        {
          char* begin = p_data + curr + 1;
          char* end = strchr(begin, '\n');
          size_t stringLength = end - begin;
          curr = curr + (uint32_t)stringLength + 1;
        }
      }
      break;
      case '\n':case '\t':case ' ': //
      default:
        break;
      }//switch

    }
    break;
    case state_t::key:
    {
      char* begin = p_data + curr;
      char* end = strchr(begin, '\"');
      size_t stringLength = end - begin;
     if (begin && end)
      {
          memcpy(m_vData[m_incNo].key, begin, stringLength % _KEY_STR_MAX);
          curr = curr + (uint32_t)stringLength;
          state = state_t::partition;
      }
    }
    break;
    case state_t::partition:
    {
      if (data == ':')
      {
        state = state_t::value;
      }
      if (data == ',')
      {
        state = state_t::section;
      }
    }
    break;
    case state_t::value:
    {
      switch (data)       // ������ ������ ���� �б�
      {
      case '"':           // ���ڰ� "�̸� ���ڿ�
      {
        char* begin = p_data + curr + 1;
        char* end = strchr(begin, '\"');
        size_t stringLength = end - begin;
        memcpy(m_vData[m_incNo].str_value, begin, stringLength % _VALUE_STR_MAX);
        curr = curr + (uint32_t)stringLength;
        //m_pData[(m_incNo++) % (m_dataMax)].type = valueType_e::string;
        m_vData[m_incNo].type = valueType_e::string;
        if ((++m_incNo < m_dataMax) == false)
        {
          ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo - 1));
          m_dataMax = m_incNo * 2;
          m_vData.reserve(m_incNo);
        }
        m_vData.push_back(json_t{});
        state = state_t::partition;
      }
      break;
      case '[':        // ���ڰ� [�̸� �迭
      {
        //parsing(p_data, size, curr);
        m_vData[m_incNo].is_array = true;
        char* begin = p_data + curr + 1;
        
        // 2022.1105 ""���� [] ���� ���Ͽ� �Ľ� ���� �߻�
        // while�� ���� double quotation ���� ] ã�� �ʰ� �Ѿ�� ��
        //char* end = strchr(begin, ']');
        char* end = begin;
        bool is_in_double_quotation = false;
        while (end++)
		{
			is_in_double_quotation = (*end == '"');
            if (is_in_double_quotation)
            {
                while (end++)
                {
                    is_in_double_quotation = (*end == '"');
                    if (is_in_double_quotation)
                    {
                        break;
                    }
                }
            }
			if (*end == ']')
			{
				break;
			}
		}

        size_t stringLength = end - begin;

        parsingArray(begin, (uint32_t)stringLength);
        curr = curr + (uint32_t)stringLength;
        state = state_t::partition;

      }
      break;
      case 't': case 'T':
      case 'f': case 'F':
      {
        char* begin = p_data + curr;
        char* end = strchr(begin, ',');
        size_t stringLength = end - begin;
        if (begin && end)
        {
            char tmp_bool[_VALUE_STR_MAX] = { 0, };
            memcpy(tmp_bool, begin, stringLength % _VALUE_STR_MAX);

            bool is_true = UTL::IsEqualStr(tmp_bool, "true", true);
            m_vData[m_incNo].yesorno = is_true;
            m_vData[m_incNo].type = valueType_e::boolian;
            if ((++m_incNo < m_dataMax) == false)
            {
                ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo - 1));
                m_dataMax = m_incNo * 2;
                m_vData.reserve(m_incNo);
            }
            m_vData.push_back(json_t{});
        }
        curr = curr + (uint32_t)stringLength;
        state = state_t::partition;


      }
      break;
      case '0': case '1': case '2': case '3': case '4': case '5':    // ���ڰ������̸�
      case '6': case '7': case '8': case '9': case '-':              // -�� ���� ��
      {
        char* begin = p_data + curr;
        char* end = strchr(begin, ',');
        if (end == 0)
        {
          // } �� ������ ���ڿ��� ����
          end = strchr(p_data + curr, '}');
          if (end == NULL)    // }�� ������ �߸��� �����̹Ƿ�
          {
            TRACE("fail json format : not found '}'! \n\r");
            return;          // �ݺ��� ����
          }
        }
        size_t stringLength = end - begin;
        char tmp_num[_VALUE_STR_MAX] = { 0, };
        memcpy(tmp_num, begin, stringLength % _VALUE_STR_MAX);
        curr = curr + (uint32_t)stringLength;

        char* dot_loc = strchr(tmp_num, '.');
        if (dot_loc != NULL)
        {
          m_vData[m_incNo].realnumber = atof(tmp_num);
          m_vData[m_incNo].type = valueType_e::realnumber;
          if ((++m_incNo < m_dataMax) == false)
          {
            ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo - 1));
            m_dataMax = m_incNo * 2;
            m_vData.reserve(m_incNo);
          }
          m_vData.push_back(json_t{});
        }
        else
        {
          m_vData[m_incNo].number = atoi(tmp_num);
          m_vData[m_incNo].type = valueType_e::integer;
          if ((++m_incNo < m_dataMax) == false)
          {
            ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo - 1));
            m_dataMax = m_incNo * 2;
            m_vData.reserve(m_incNo);
          }
          m_vData.push_back(json_t{});
        }
        state = state_t::partition;
        is_pass_cnt = true;
      }
      break;
      default:
        break;
      }
    }
    break;
    }//switch


    if (is_pass_cnt)
      is_pass_cnt = false;
    else
      curr++;

  }//while

#endif
}

void parser::json::parsingArray(char* p_data, uint32_t size)
{
  enum class state_t {
    section,
    key,
    value,
    partition,
  };

  //char tmp[256] = { 0, };
  std::vector<char> tmp((size_t)size + 1);
  memcpy(tmp.data(), p_data, tmp.size());
  vector<char*> tmp_p(_ARG_CNT_MAX);
  state_t state = state_t::section;
  std::array<char, _KEY_STR_MAX> key_str;
  memcpy(key_str.data(), m_vData[m_incNo].key, _KEY_STR_MAX);

  uint8_t cnt = trans::SplitArgs(tmp.data(), tmp_p.data(), ",", _ARG_CNT_MAX);

  for (int i = 0; i < cnt; i++)
  {
    char data;
    uint32_t t_curr = 0;
    uint32_t t_arg_size = 0;
    if (tmp_p[i] != NULL)
    {
      uint32_t t_arg_size = (uint32_t)strlen(tmp_p[i]);
      while (t_curr < t_arg_size)
      {
        data = tmp_p[i][t_curr];
        // fine key and value
        {
          switch (data)       // ������ ������ ���� �б�
          {
          case ';':
            __fallthrough;
          case '/':           // �ּ� Ȯ�� �� ���๮�� Ȯ�α��� 
          {
            char* begin = tmp_p[i] + t_curr + 1;
            char* end = strchr(begin, '\n');
            if (begin && end)
            {
                size_t stringLength = end - begin;
                t_curr = t_curr + (uint32_t)stringLength + 1;
            }
          }
          break;
          case '"':           // ���ڰ� "�̸� ���ڿ�
          {
            memcpy(m_vData[m_incNo].key, key_str.data(), key_str.size());
            m_vData[m_incNo].is_array = true;
            char* begin = tmp_p[i] + 1;
            char* end = strchr(begin, '\"');
            if (end && begin)
            {   
                size_t stringLength = end - begin;
                memcpy(m_vData[m_incNo].str_value, begin, stringLength% _VALUE_STR_MAX);
                m_vData[m_incNo].type = valueType_e::string;
                if ((++m_incNo < m_dataMax) == false)
                {
                    ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo - 1));
                    m_dataMax = m_incNo * 2;
                    m_vData.reserve(m_dataMax);
                }
                m_vData.push_back(json_t{});
            }           
            t_curr = t_arg_size;
          }
          break;
          case '{':        // key- value       
          {
            memcpy(m_vData[m_incNo].key, key_str.data(), key_str.size());
            m_vData[m_incNo].is_array = true;
            m_vData[m_incNo].key_value = new key_t();
            m_vData[m_incNo].type = valueType_e::key_value;

            uint32_t curr = 1;
            uint32_t arg_size = (uint32_t)strlen(tmp_p[i]);
            state = state_t::section;
            while (curr < arg_size)
            {
              char data = tmp_p[i][curr];
              switch (state)
              {
              case state_t::section:
              {
                switch (data)
                {
                case '\"':
                  state = state_t::key;
                  break;

                default:
                  break;
                }//switch (data)
              }
              break;
              case state_t::key:
              {
                char* begin = tmp_p[i] + curr;
                char* end = strchr(begin, '\"');
                size_t stringLength = end - begin;
                memcpy(m_vData[m_incNo].key_value->key, begin, stringLength % _KEY_STR_MAX);
                curr = curr + (uint32_t)stringLength;
                state = state_t::partition;
              }
              break;
              case state_t::partition:
              {
                if (data == ':')
                {
                  state = state_t::value;
                }
              }
              break;
              case state_t::value:
              {
                switch (data)       // ������ ������ ���� �б�
                {
                case '"':           // ���ڰ� "�̸� ���ڿ�
                {
                  char* begin = tmp_p[i] + curr + 1;
                  char* end = strchr(begin, '\"');
                  size_t stringLength = end - begin;
                  memcpy(m_vData[m_incNo].key_value->str_value, begin, stringLength % _VALUE_STR_MAX);
                  m_vData[m_incNo].key_value->type = valueType_e::string;
                  if ((++m_incNo < m_dataMax) == false)
                  {
                    ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo-1));
                    m_dataMax = m_incNo * 2;
                    m_vData.reserve(m_dataMax);
                  }
                  m_vData.push_back(json_t{});
                  curr = arg_size;
                  t_curr = t_arg_size;

                }
                break;
                case '[':        // ���ڰ� [�̸� �迭
                  break;

                case 't': case 'T':
                case 'f': case 'F':
                {
                  char* begin = tmp_p[i] + curr;
                  char* end = strchr(begin, '}');
                  if (end  && begin)
                  {
                      size_t stringLength = end - begin;
                      char tmp_bool[_VALUE_STR_MAX] = { 0, };
                      memcpy(tmp_bool, begin, stringLength% _VALUE_STR_MAX);

                      bool is_true = UTL::IsEqualStr(tmp_bool, "true", true);
                      m_vData[m_incNo].key_value->yesorno = is_true;
                      m_vData[m_incNo].key_value->type = valueType_e::boolian;
                      if ((++m_incNo < m_dataMax) == false)
                      {
                          ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo - 1));
                          m_dataMax = m_incNo * 2;
                          m_vData.reserve(m_dataMax);
                      }
                      m_vData.push_back(json_t{});
                  } 
                  else
                  {
                      end = 0;
                  }
                  curr = arg_size;
                  t_curr = t_arg_size;
                }
                break;
                case '0': case '1': case '2': case '3': case '4': case '5':    // ���ڰ������̸�
                case '6': case '7': case '8': case '9': case '-':              // -�� ���� ��
                {
                  char* begin = tmp_p[i] + curr;
                  char* end = strchr(begin, '}');
                  if (end && begin)
                  {
                      size_t stringLength = end - begin;
                      char tmp_num[_VALUE_STR_MAX] = { 0, };
                      memcpy(tmp_num, begin, stringLength % _VALUE_STR_MAX);

                      char* dot_loc = strchr(tmp_num, '.');
                      if (dot_loc != NULL)
                      {
                          m_vData[m_incNo].key_value->realnumber = atof(tmp_num);
                          m_vData[m_incNo].key_value->type = valueType_e::realnumber;
                          if ((++m_incNo < m_dataMax) == false)
                          {
                              ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo - 1));
                              m_dataMax = m_incNo * 2;
                              m_vData.reserve(m_dataMax);
                          }
                          m_vData.push_back(json_t{});
                      }
                      else
                      {
                          m_vData[m_incNo].key_value->number = atoi(tmp_num);
                          m_vData[m_incNo].key_value->type = valueType_e::integer;
                          if ((++m_incNo < m_dataMax) == false)
                          {
                              ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo - 1));
                              m_dataMax = m_incNo * 2;
                              m_vData.reserve(m_dataMax);
                          }
                          m_vData.push_back(json_t{});
                      }
                  }
                  curr = arg_size;
                  t_curr = t_arg_size;
                }
                break;
                default:
                  break;
                }// switch (data) 
              }
              break;
              default:
                break;
              }//switch (state)

              curr++;
            }//while
          }
          break;
          case 't': case 'T':
          case 'f': case 'F':
          {
            memcpy(m_vData[m_incNo].key, key_str.data(), key_str.size());
            m_vData[m_incNo].is_array = true;
            bool is_true = UTL::IsEqualStr(tmp_p[i], "true", true);
            m_vData[m_incNo].yesorno = is_true;
            m_vData[m_incNo].type = valueType_e::boolian;
            if ((++m_incNo < m_dataMax) == false)
            {
              ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo - 1));
              m_dataMax = m_incNo * 2;
              m_vData.reserve(m_dataMax);
            }
            m_vData.push_back(json_t{});
            t_curr = t_arg_size;
          }
          break;
          case '0': case '1': case '2': case '3': case '4': case '5':    // ���ڰ������̸�
          case '6': case '7': case '8': case '9': case '-':              // -�� ���� ��
          {
            memcpy(m_vData[m_incNo].key, key_str.data(), key_str.size());
            m_vData[m_incNo].is_array = true;

            char* dot_loc = strchr(tmp_p[i], '.');
            if (dot_loc != NULL)
            {
              m_vData[m_incNo].realnumber = atof(tmp_p[i]);
              m_vData[m_incNo].type = valueType_e::realnumber;
              if ((++m_incNo < m_dataMax) == false)
              {
                ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo - 1));
                m_dataMax = m_incNo * 2;
                m_vData.reserve(m_dataMax);
              }
              m_vData.push_back(json_t{});
            }
            else
            {
              m_vData[m_incNo].number = atoi(tmp_p[i]);
              m_vData[m_incNo].type = valueType_e::integer;
              if ((++m_incNo < m_dataMax) == false)
              {
                ERR_PRINT("parser::json ig m_dataMax(%d)�� ���� m_incNo(%d) �ʰ� �Ǿ����ϴ�.", m_dataMax, (m_incNo - 1));
                m_dataMax = m_incNo * 2;
                m_vData.reserve(m_dataMax);
              }
              m_vData.push_back(json_t{});
            }
            t_curr = t_arg_size;
          }
          break;
          default:
            break;
          }

        }
        t_curr++;
      }// while (arg_size < curr)    
    }//if (tmp_p[i] != NULL)
  }//for (int i = 0; i < cnt; i++)
}

bool parser::json::putData(char** pdata, uint8_t cnt)
{
  return false;
}
