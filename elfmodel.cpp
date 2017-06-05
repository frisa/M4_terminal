#include <QFile>
#include <QDebug>

// unix header files
#include <fcntl.h>
#include <stdlib.h>
#include "elfmodel.h"
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <elf.h>

extern "C"
{
    extern void read_elf_header(int32_t fd, Elf32_Ehdr *elf_header);
    extern void read_section_header_table(int32_t fd, Elf32_Ehdr eh, Elf32_Shdr sh_table[]);
    extern char * read_section(int32_t fd, Elf32_Shdr sh);
}


ElfFile::ElfFile()
{
    m_name = "Default";
}

ElfListModel::ElfListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void ElfListModel::log(QString message)
{
    m_log = m_log+ message + "\n";
}

void ElfListModel::readElfHeader(int32_t fd, Elf32_Ehdr *elf_header)
{
    assert(elf_header != NULL);
    assert(lseek(fd, (off_t)0, SEEK_SET) == (off_t)0);
    assert(read(fd, (void *)elf_header, sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));
}

void ElfListModel::readrSectionHeaderTable(int32_t fd, Elf32_Ehdr eh, Elf32_Shdr sh_table[])
{
    uint32_t i;
    assert(lseek(fd, (off_t)eh.e_shoff, SEEK_SET) == (off_t)eh.e_shoff);
    for(i=0; i<eh.e_shnum; i++) {
        assert(read(fd, (void *)&sh_table[i], eh.e_shentsize) == eh.e_shentsize);
    }
}

char * ElfListModel::readReadSection(int32_t fd, Elf32_Shdr sh)
{
    char* buff = (char*) malloc(sh.sh_size);
    if(!buff) {
        qDebug("%s:Failed to allocate %dbytes\n", __func__, sh.sh_size);
    }
    assert(buff != NULL);
    assert(lseek(fd, (off_t)sh.sh_offset, SEEK_SET) == (off_t)sh.sh_offset);
    assert(read(fd, (void *)buff, sh.sh_size) == sh.sh_size);
    return buff;
}

void ElfListModel::loadElfHeader(Elf32_Ehdr elf_header, ElfFile *elfFile)
{
    QString sVariable, sParamenter;
    /* Storage capacity class */
    switch(elf_header.e_ident[EI_CLASS])
    {
        case ELFCLASS32:
            sVariable = "32-bit objects";
            break;

        case ELFCLASS64:
            sVariable = "64-bit objects";
            break;

        default:
            sVariable = "INVALID CLASS";
            break;
    }
    sParamenter = "Storage class\t= ";
    qDebug("%s %s", qUtf8Printable(sParamenter), qUtf8Printable(sVariable));
    elfFile->m_sclass = sVariable;

    switch(elf_header.e_ident[EI_DATA])
    {
        case ELFDATA2LSB:
            sVariable = "2's complement, little endian";
            break;

        case ELFDATA2MSB:
            sVariable = "2's complement, big endian";
            break;

        default:
            sVariable = "INVALID Format";
            break;
    }
    sParamenter = "Data format\t= ";
    qDebug("%s %s", qUtf8Printable(sParamenter), qUtf8Printable(sVariable));
    elfFile->m_dformat = sVariable;

    switch(elf_header.e_ident[EI_OSABI])
    {
        case ELFOSABI_SYSV:
            sVariable = "UNIX System V ABI";
            break;

        case ELFOSABI_HPUX:
            sVariable = "HP-UX";
            break;

        case ELFOSABI_NETBSD:
            sVariable = "NetBSD";
            break;

        case ELFOSABI_LINUX:
            sVariable = "Linux";
            break;

        case ELFOSABI_SOLARIS:
            sVariable = "Sun Solaris";
            break;

        case ELFOSABI_AIX:
            sVariable = "IBM AIX";
            break;

        case ELFOSABI_IRIX:
            sVariable = "SGI Irix";
            break;

        case ELFOSABI_FREEBSD:
            sVariable = "FreeBSD";
            break;

        case ELFOSABI_TRU64:
            sVariable = "Compaq TRU64 UNIX";
            break;

        case ELFOSABI_MODESTO:
            sVariable = "Novell Modesto";
            break;

        case ELFOSABI_OPENBSD:
            sVariable = "OpenBSD";
            break;

        case ELFOSABI_ARM_AEABI:
            sVariable = "ARM EABI";
            break;

        case ELFOSABI_ARM:
            sVariable = "ARM";
            break;

        case ELFOSABI_STANDALONE:
            sVariable = "Standalone (embedded) app";
            break;

        default:
            sVariable = "Unknown (0x%x)";
            break;
    }
    sParamenter = "OS ABI\t\t= ";
    qDebug("%s %s", qUtf8Printable(sParamenter), qUtf8Printable(sVariable));
    elfFile->m_abi = sVariable;

    switch(elf_header.e_type)
    {
        case ET_NONE:
            sVariable = "N/A (0x0)";
            break;

        case ET_REL:
            sVariable = "Relocatable";
            break;

        case ET_EXEC:
            sVariable = "Executable";
            break;

        case ET_DYN:
            sVariable = "Shared Object";
            break;
        default:
            sVariable = "Unknown (0x%x)";
            break;
    }
    sParamenter = "Filetype \t= ";
    qDebug("%s %s", qUtf8Printable(sParamenter), qUtf8Printable(sVariable));
    elfFile->m_type = sVariable;

    switch(elf_header.e_machine)
    {
        case EM_NONE:
            sVariable = "None (0x0)";
            break;

        case EM_386:
            sVariable = QString("INTEL x86 (0x%x)").arg(EM_386);
            break;

        case EM_ARM:
            sVariable = QString("ARM (%1)").arg(EM_ARM);
            break;
        default:
            sVariable = QString("Machine\t= %1").arg(elf_header.e_machine);
            break;
    }
    sParamenter = "Machine\t\t= ";
    qDebug("%s %s", qUtf8Printable(sParamenter), qUtf8Printable(sVariable));
    elfFile->m_machine = sVariable;

    /* Entry point */
    qDebug("Entry point\t= 0x%08x", elf_header.e_entry);
    elfFile->m_entry = elf_header.e_entry;

    /* ELF header size in bytes */
    qDebug("ELF header size\t= 0x%08x", elf_header.e_ehsize);
    elfFile->m_hsize = elf_header.e_ehsize;

    /* Program Header */
    qDebug("Program Header\t= 0x%08x", elf_header.e_phoff); /* start */
    qDebug("\t\t  %d entries", elf_header.e_phnum);	/* num entry */
    qDebug("\t\t  %d bytes", elf_header.e_phentsize);	/* size/entry */

    elfFile->m_pheader = elf_header.e_phoff;
    elfFile->m_pheader_entries = elf_header.e_phnum;
    elfFile->m_pheader_size = elf_header.e_phentsize;

    /* Section header starts at */
    qDebug("Section Header\t= 0x%08x", elf_header.e_shoff); /* num entry */
    qDebug("\t\t  %d entries", elf_header.e_shnum);
    qDebug("\t\t  %d bytes", elf_header.e_shentsize);	/* size/entry */

    elfFile->m_sheader = elf_header.e_phoff;
    elfFile->m_sheader_entries = elf_header.e_phnum;
    elfFile->m_sheader_size = elf_header.e_phentsize;

    qDebug("\t\t  0x%08x (string table offset)", elf_header.e_shstrndx);

    /* File flags (Machine specific)*/
    qDebug("File flags \t= 0x%08x", elf_header.e_flags);
    elfFile->m_flags = elf_header.e_flags;

    /* ELF file flags are machine specific.
     * INTEL implements NO flags.
     * ARM implements a few.
     * Add support below to parse ELF file flags on ARM
     */
    int32_t ef = elf_header.e_flags;
    qDebug("\t\t  ");
    sVariable = "";
    if(ef & EF_ARM_RELEXEC)
        sVariable.append(",RELEXEC ");

    if(ef & EF_ARM_HASENTRY)
        sVariable.append(",HASENTRY ");

    if(ef & EF_ARM_INTERWORK)
        sVariable.append(",INTERWORK ");

    if(ef & EF_ARM_APCS_26)
        sVariable.append(",APCS_26 ");

    if(ef & EF_ARM_APCS_FLOAT)
        sVariable.append(",APCS_FLOAT ");

    if(ef & EF_ARM_PIC)
        sVariable.append(",PIC ");

    if(ef & EF_ARM_ALIGN8)
        sVariable.append(",ALIGN8 ");

    if(ef & EF_ARM_NEW_ABI)
        sVariable.append(",NEW_ABI ");

    if(ef & EF_ARM_OLD_ABI)
        sVariable.append(",OLD_ABI ");

    if(ef & EF_ARM_SOFT_FLOAT)
        sVariable.append(",SOFT_FLOAT ");

    if(ef & EF_ARM_VFP_FLOAT)
        sVariable.append(",VFP_FLOAT ");

    if(ef & EF_ARM_MAVERICK_FLOAT)
        sVariable.append(",MAVERICK_FLOAT ");
    /* MSB of flags conatins ARM EABI version */
    qDebug("FLAGS: %s", qUtf8Printable(sVariable));
    qDebug("ARM EABI\t= Version %d", (ef & EF_ARM_EABIMASK)>>24);
}

void ElfListModel::loadElfSectionHeader(int32_t fd, Elf32_Ehdr eh, Elf32_Shdr sh_table[])
{
    uint32_t i;
    char* sh_str;	/* section-header string-table is also a section. */

    /* Read section-header string-table */
    sh_str = readReadSection(fd, sh_table[eh.e_shstrndx]);

    qDebug("================================================================================");
    log(QString("idx\toffset\tload-addr\tsize\talgn\tflags\ttype\tsection"));
    qDebug(" idx offset     load-addr  size       algn"         " flags      type       section\n");
    qDebug("================================================================================");

    for(i=0; i<eh.e_shnum; i++) {
        qDebug(" %03d 0x%08x 0x%08x 0x%08x %4d 0x%08x 0x%08x %s\t", i, sh_table[i].sh_offset, sh_table[i].sh_addr, sh_table[i].sh_size
               , sh_table[i].sh_addralign, sh_table[i].sh_flags, sh_table[i].sh_type, (sh_str + sh_table[i].sh_name));
        log(QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8").arg(i).arg(sh_table[i].sh_offset).arg(sh_table[i].sh_addr).arg(sh_table[i].sh_size).arg(sh_table[i].sh_addralign).arg(sh_table[i].sh_flags).arg(sh_table[i].sh_type).arg((sh_str + sh_table[i].sh_name)));
    }
    qDebug("================================================================================");
}

void ElfListModel::loadSymbolTable(int32_t fd, Elf32_Shdr sh_table[], uint32_t symbol_table)
{
    char *str_tbl;
    Elf32_Sym* sym_tbl;
    uint32_t i, symbol_count;

    sym_tbl = (Elf32_Sym*)readReadSection(fd, sh_table[symbol_table]);

    /* Read linked string-table
     * Section containing the string table having names of
     * symbols of this section
     */
    uint32_t str_tbl_ndx = sh_table[symbol_table].sh_link;
    str_tbl = readReadSection(fd, sh_table[str_tbl_ndx]);

    symbol_count = (sh_table[symbol_table].sh_size/sizeof(Elf32_Sym));
    qDebug("%d symbols\n", symbol_count);

    for(i=0; i< symbol_count; i++) {
        qDebug("0x%08x 0x%02x 0x%02x %s",
               sym_tbl[i].st_value, ELF32_ST_BIND(sym_tbl[i].st_info),
               ELF32_ST_TYPE(sym_tbl[i].st_info), (str_tbl + sym_tbl[i].st_name));
    }
}

void ElfListModel::loadSymbols(int32_t fd, Elf32_Ehdr eh, Elf32_Shdr sh_table[])
{
    uint32_t i;
    for(i=0; i<eh.e_shnum; i++) {
        if ((sh_table[i].sh_type==SHT_SYMTAB)
         || (sh_table[i].sh_type==SHT_DYNSYM)) {
            qDebug("\n[Section %03d]", i);
            loadSymbolTable(fd, sh_table, i);
        }
    }
}

void ElfListModel::saveTextSection(int32_t fd, Elf32_Ehdr eh, Elf32_Shdr sh_table[])
{
    uint32_t i;
    int32_t fd2;	/* to write text.S in current directory */
    char* sh_str;	/* section-header string-table is also a section. */
    char* buf;      /* buffer to hold contents of the .text section */

    char *pwd = getcwd(NULL, (size_t)NULL);
    printf("%s\n", pwd);
    pwd = (char*)realloc(pwd, strlen(pwd)+8);
    strcat(pwd,"/text.S");
    qDebug("%s", pwd);

    /* Read section-header string-table */
    sh_str = readReadSection(fd, sh_table[eh.e_shstrndx]);

    for(i=0; i<eh.e_shnum; i++) {
        if(!strcmp(".text", (sh_str + sh_table[i].sh_name))) {
            qDebug("Found section\t\".text\" at offset\t0x%08x of size\t\t0x%08x", sh_table[i].sh_offset, sh_table[i].sh_size);
            break;
        }
    }

    assert(lseek(fd, sh_table[i].sh_offset, SEEK_SET)==sh_table[i].sh_offset);
    buf = (char*)malloc(sh_table[i].sh_size);
    if(!buf) {
        printf("Failed to allocate %dbytes!!\n", sh_table[i].sh_size);
        free(pwd);
    }
    assert(read(fd, buf, sh_table[i].sh_size)==sh_table[i].sh_size);
    fd2 = open(pwd, O_RDWR|O_SYNC|O_CREAT);
    write(fd2, buf, sh_table[i].sh_size);
    fsync(fd2);
    close(fd2);
    free(pwd);
}

void ElfListModel::addElfFile(QString sPath, ElfFile *elfFile)
{
   int32_t fd;
   Elf32_Ehdr eh;		/* elf-header is fixed size */
   Elf32_Shdr* sh_tbl;	/* section-header table is variable size */

   fd = open(sPath.toLatin1().data(), O_RDONLY|O_SYNC);
   if (0 > fd)
   {
       qDebug("The file %s can not be open", qUtf8Printable(sPath));
   }
   else
   {
       elfFile->m_name = sPath;

       qDebug("The file %s has been open", qUtf8Printable(sPath));
       log(QString("Loaded: %1").arg(sPath));
       readElfHeader(fd, &eh);
       loadElfHeader(eh, elfFile);

       sh_tbl = (Elf32_Shdr*) malloc(eh.e_shentsize * eh.e_shnum);
       if(!sh_tbl)
       {
           qDebug("Failed to allocate %d bytes\n", (eh.e_shentsize * eh.e_shnum));
       }

       readrSectionHeaderTable(fd, eh, sh_tbl);
       loadElfSectionHeader(fd, eh, sh_tbl);
       loadSymbols(fd, eh, sh_tbl);
       saveTextSection(fd, eh, sh_tbl);

       beginInsertRows(QModelIndex(), rowCount(), rowCount());
       m_elfFiles << elfFile;
       endInsertRows();
   }
   //disassemble(fd, eh, sh_tbl);
}

int ElfListModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_elfFiles.count();
}

QVariant ElfListModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_elfFiles.count())
        return QVariant();

    const ElfFile * elfFile = m_elfFiles[index.row()];
    if (role == NameRole)
        return elfFile->m_name;
    else if (role == SclassRole)
        return elfFile->m_sclass;
    else if (role == DformatRole)
        return elfFile->m_dformat;
    else if (role == AbiRole)
        return elfFile->m_abi;
    else if (role == TypeRole)
        return elfFile->m_type;
    else if (role == MachineRole)
        return elfFile->m_machine;
    else if (role == EntryRole)
        return elfFile->m_entry;
    else if (role == HsizeRole)
        return elfFile->m_hsize;
    return QVariant();
}

QHash<int, QByteArray> ElfListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[SclassRole] = "sclass";
    roles[DformatRole] = "dformat";
    roles[AbiRole] = "abi";
    roles[TypeRole] = "type";
    roles[MachineRole] = "machine";
    roles[EntryRole] = "entry";
    roles[HsizeRole] = "hsize";
    return roles;
}
