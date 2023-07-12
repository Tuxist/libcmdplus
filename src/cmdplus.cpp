/*******************************************************************************
Copyright (c) 2021, Jan Koester jan.koester@gmx.net
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include <cstdio>
#include <cstring>
#include <iostream>

#include "cmdplus.h"

#define KTKEY 0
#define KTSKEY 1

cmdplus::Cmd::Cmd() {
    _Key = nullptr;
    _SKey = '\0';
    _Value = nullptr;
    _Help = nullptr;
    _Found = false;
    _Required = false;
    _nextCmd = nullptr;
}

const char *cmdplus::Cmd::getKey() {
    return _Key;
}

const char cmdplus::Cmd::getShortkey() {
    return _SKey;
}

const char *cmdplus::Cmd::getValue() {
    return _Value;
}

unsigned long cmdplus::Cmd::getValueSize_t() {
    return atoi(_Value);
}

int cmdplus::Cmd::getValueInt() {
    return atoi(_Value);
}

const char *cmdplus::Cmd::getHelp() {
    return _Help;
}

bool cmdplus::Cmd::getFound() {
    return _Found;
}

bool cmdplus::Cmd::getRequired() {
    return _Required;
}

cmdplus::Cmd *cmdplus::Cmd::nextCmd() {
    return _nextCmd;
}

cmdplus::Cmd::~Cmd() {
    delete[] _Key;
    delete[] _Value;
    delete[] _Help;
    delete _nextCmd;
}

cmdplus::CmdController::CmdController() {
    _firstCmd = nullptr;
    _lastCmd = nullptr;
}

void cmdplus::CmdController::registerCmd(const char *key, const char skey,bool required, const char *defaultvalue, const char *help) {
    if (!key || !skey || !help) {
        throw "cmd parser key,skey or help not set!";
    }
    /*if key exist overwriting options*/
    for (Cmd *curdcmd = _firstCmd; curdcmd; curdcmd=curdcmd->nextCmd()) {
        if (strcmp(key,curdcmd->getKey()) == 0) {
            /*set new shortkey*/
            curdcmd->_SKey = skey;
            /*set reqirement flag*/
            curdcmd->_Required = required;
            /*set new value*/
            delete[] curdcmd->_Value;
            curdcmd->_Value = new char[strlen(defaultvalue)+1];
            memcpy(curdcmd->_Value,defaultvalue,strlen(defaultvalue));
            curdcmd->_Value[strlen(defaultvalue)] = '\0';
            /*set new help*/
            delete[] curdcmd->_Help;
            curdcmd->_Help = new char[strlen(help) + 1];
            memcpy(curdcmd->_Help,help,strlen(help));
            curdcmd->_Help[strlen(help)] = '\0';
            return;
        }
    }
    /*create new key value store*/
    if (!_firstCmd) {
        _firstCmd = new Cmd;
        _lastCmd = _firstCmd;
    }
    else {
        _lastCmd->_nextCmd = new Cmd;
        _lastCmd = _lastCmd->_nextCmd;
    }
    /*set new key*/
    _lastCmd->_Key = new char[strlen(key) + 1];
    memcpy(_lastCmd->_Key,key,strlen(key));
    _lastCmd->_Key[strlen(key)] = '\0';
    /*set new shortkey*/
    _lastCmd->_SKey = skey;
    /*set reqirement flag*/
    _lastCmd->_Required = required;
    /*set new value*/
    if (defaultvalue) {
        _lastCmd->_Value = new char[strlen(defaultvalue) + 1];
        memcpy(_lastCmd->_Value,defaultvalue,strlen(defaultvalue));
        _lastCmd->_Value[strlen(defaultvalue)] = '\0';
    }
    /*set new help*/
    _lastCmd->_Help = new char[strlen(help) + 1];
    memcpy(_lastCmd->_Help,help,strlen(help));
    _lastCmd->_Help[strlen(help)] = '\0';
    
}

void cmdplus::CmdController::registerCmd(const char *key, const char skey, bool required, unsigned long defaultvalue, const char *help) {
    char buf[255];
    snprintf(buf,255,"%lu",defaultvalue);
    registerCmd(key,skey,required,buf,help);
}

void cmdplus::CmdController::registerCmd(const char *key, const char skey, bool required, int defaultvalue, const char *help) {
    char buf[255];
    snprintf(buf,255,"%d",defaultvalue);                         
    registerCmd(key, skey, required, buf, help);
}

void cmdplus::CmdController::parseCmd(int argc, char** argv){
    for (int args = 1; args < argc; args++) {
        int keytype = -1;
        if (argv[args][0]=='-' && argv[args][1] == '-') {
            keytype = KTKEY;
        }else if (argv[args][0] == '-'){
            keytype = KTSKEY;
        }else {
            break;
        }
        
        unsigned long kendpos = strlen(argv[args]);
        for (unsigned long cmdpos = 0; cmdpos < strlen(argv[args])+1; cmdpos++) {	
            switch (argv[args][cmdpos]) {
                case '=': {
                    kendpos = cmdpos;
                };
            }
        }
        
        char *key = nullptr;
        char skey = '0';
        if (keytype == KTKEY) {
            key = new char[kendpos-1];
            memcpy(key,argv[args] +2,kendpos-2);
            key[kendpos - 2] = '\0';
        } else if (keytype == KTSKEY){
            skey = argv[args][1];
        }
        
        for (Cmd *curcmd = _firstCmd; curcmd; curcmd = curcmd->nextCmd()) {
            if (keytype == KTKEY) {
                if (strcmp(key,curcmd->getKey()) == 0) {
                    curcmd->_Found = true;
                    int valuesize = (strlen(argv[args]) - (kendpos+1));
                    if (valuesize > 0) {
                        delete[] curcmd->_Value;
                        curcmd->_Value = new char[valuesize+1];
                        memcpy(curcmd->_Value,argv[args]+(kendpos+1),(strlen(argv[args])-(kendpos+1)));
                        curcmd->_Value[valuesize] = '\0';
                    }
                }
            } else if (keytype == KTSKEY) {
                if (curcmd->getShortkey()== skey) {
                    curcmd->_Found = true;
                    if (++args<argc) {
                        int valuesize = strlen(argv[args]);
                        delete[] curcmd->_Value;
                        curcmd->_Value = new char[valuesize + 1];
                        memcpy(curcmd->_Value,argv[args],strlen(argv[args]));
                        curcmd->_Value[valuesize] = '\0';
                    }
                }
            }
        }
        
        delete[] key;
    }
}

bool cmdplus::CmdController::checkRequired() {
    for (Cmd *curdcmd = _firstCmd; curdcmd; curdcmd = curdcmd->nextCmd()) {
        if (curdcmd->getRequired() && !curdcmd->_Found) {
            return false;
        }
    }
    return true;
}

void cmdplus::CmdController::printHelp() {
    for (Cmd *curdcmd = _firstCmd; curdcmd; curdcmd = curdcmd->nextCmd()) {
        std::cout << "--" << curdcmd->getKey()
                                     << " -" << curdcmd->getShortkey()
                                     << " "  << curdcmd->getHelp() 
                                     << std::endl;
    }
}

cmdplus::Cmd *cmdplus::CmdController::getCmdbyKey(const char *key) {
    for (Cmd *curdcmd = _firstCmd; curdcmd; curdcmd = curdcmd->nextCmd()) {
        if (strcmp(key,curdcmd->getKey()) == 0) {
            return curdcmd;
        }
    }
    return nullptr;
}

cmdplus::CmdController::~CmdController() {
    delete _firstCmd;
    _lastCmd = nullptr;
}
